/* cache.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 29 Sep 2017, 18:39:38 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2017  Trinity Annabelle Quirk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * This file contains a template class for doing cache hashes for
 * arbitrary types of objects.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_CACHE_H__
#define __INC_CUDDLY_CACHE_H__

#include <config.h>

#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

#include <cstdint>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <chrono>

template <typename T>
struct noop_cleanup
{
    void operator()(T& o) { return; }
};

template <typename obj_type,
          typename cleanup = noop_cleanup<obj_type>,
          typename key_type = uint64_t>
class BasicCache
{
  protected:
    typedef BasicCache<obj_type, cleanup, key_type> _bct;
    typedef std::chrono::steady_clock _bc_time;
    typedef std::chrono::time_point<typename _bct::_bc_time> _bc_tpoint;
    typedef struct object_timeval_tag
    {
        _bc_tpoint lastused;
        obj_type obj;
    }
    _bcos_t;
    typedef std::unordered_map<key_type, typename _bct::_bcos_t> _bcom_t;

    struct bc_cleanup
    {
        void operator()(typename _bct::_bcos_t& val)
            {
                std::function<void(obj_type&)> cleanup_func = cleanup();
                cleanup_func(val.obj);
            }
    };
    typedef std::function<void(typename _bct::_bcos_t&)> clean_func_type;

    const static int PRUNE_INTERVAL = 600;
    const static int PRUNE_LIFETIME = 1200;

    typename _bct::_bcom_t _bc_map;
    std::string type;
    pthread_t prune_thread;

    static void *prune_worker(void *arg)
        {
            _bct *bc = (_bct *)arg;
            _bc_tpoint limit;
            std::chrono::seconds obj_lifetime(_bct::PRUNE_LIFETIME);
            typename _bct::_bcom_t::iterator i;
            std::vector<typename _bct::_bcom_t::iterator> old_elems;
            typename std::vector<typename _bct::_bcom_t::iterator>::iterator j;

            for (;;)
            {
                sleep(_bct::PRUNE_INTERVAL);
                limit = _bc_time::now() - obj_lifetime;
                old_elems.clear();
                for (i = bc->_bc_map.begin(); i != bc->_bc_map.end(); ++i)
                    if (i->first != 0LL && i->second.lastused >= limit)
                        old_elems.push_back(i);

                if (old_elems.size() > 0)
                {
                    typename _bct::clean_func_type cleanup_func = bc_cleanup();
                    for (j = old_elems.begin(); j != old_elems.end(); ++j)
                    {
                        cleanup_func((*j)->second);
                        bc->_bc_map.erase(*j);
                    }
                    std::clog << "Removed " << old_elems.size()
                              << " entities from " << bc->type
                              << " cache" << std::endl;
                }
            }
        };

  public:
    BasicCache(const std::string type_name)
        : _bc_map(), type(type_name)
        {
            int ret;

            if ((ret = pthread_create(&(this->prune_thread),
                                      NULL,
                                      this->prune_worker,
                                      (void *)this)) != 0)
            {
                std::ostringstream s;
                s << "Couldn't start " << this->type
                  << " reaper thread: " << strerror(ret) << " (" << ret << ")";
                throw std::runtime_error(s.str());
            }
        };
    ~BasicCache()
        {
            int ret;
            typename _bct::_bcom_t::iterator i;

            if ((ret = pthread_cancel(this->prune_thread)) != 0)
                std::clog << "Couldn't cancel " << this->type
                          << " reaper thread: "
                          << strerror(ret) << " (" << ret << ")" << std::endl;
            sleep(0);
            if ((ret = pthread_join(this->prune_thread, NULL)) != 0)
                std::clog << "Couldn't reap " << this->type
                          << " reaper thread: "
                          << strerror(ret) << " (" << ret << ")" << std::endl;

            typename _bct::clean_func_type cleanup_func = bc_cleanup();

            for (i = this->_bc_map.begin(); i != this->_bc_map.end(); ++i)
                cleanup_func(i->second);
        };
    virtual obj_type& operator[](key_type objid)
        {
            _bcos_t& item = this->_bc_map[objid];

            item.lastused = _bc_time::now();
            return item.obj;
        };
    void erase(key_type objid)
        {
            typename _bct::_bcom_t::iterator object = this->_bc_map.find(objid);

            if (object != this->_bc_map.end())
            {
                typename _bct::clean_func_type cleanup_func = bc_cleanup();

                cleanup_func(object->second);
                this->_bc_map.erase(object);
            }
        };
    void each(std::function<void(obj_type&)> func)
        {
            std::for_each(this->_bc_map.begin(),
                          this->_bc_map.end(),
                          [&] (typename _bct::_bcom_t::value_type& v)
                            { func(v.second.obj); });
        };
};

#endif /* __INC_CUDDLY_CACHE_H__ */
