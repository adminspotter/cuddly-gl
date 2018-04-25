/* bidi_classes.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 25 Apr 2018, 07:02:16 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2018  Trinity Annabelle Quirk
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
 * Character classes for the Unicode BIDI algorithm.
 *
 * Things to do
 *
 */

#include "bidi.h"

/* Class AL:  Arabic Letters */
std::unordered_set<char32_t> AL
{
    0x0608, 0x060b, 0x060d, 0x061b, 0x061c, 0x061e, 0x061f, 0x0620,
    0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627, 0x0628,
    0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f, 0x0630,
    0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x0637, 0x0638,
    0x0639, 0x063a, 0x063b, 0x063c, 0x063d, 0x063e, 0x063f, 0x0640,
    0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647, 0x0648,
    0x0649, 0x064a, 0x066d, 0x066e, 0x066f, 0x0671, 0x0672, 0x0673,
    0x0674, 0x0675, 0x0676, 0x0677, 0x0678, 0x0679, 0x067a, 0x067b,
    0x067c, 0x067d, 0x067e, 0x067f, 0x0680, 0x0681, 0x0682, 0x0683,
    0x0684, 0x0685, 0x0686, 0x0687, 0x0688, 0x0689, 0x068a, 0x068b,
    0x068c, 0x068d, 0x068e, 0x068f, 0x0690, 0x0691, 0x0692, 0x0693,
    0x0694, 0x0695, 0x0696, 0x0697, 0x0698, 0x0699, 0x069a, 0x069b,
    0x069c, 0x069d, 0x069e, 0x069f, 0x06a0, 0x06a1, 0x06a2, 0x06a3,
    0x06a4, 0x06a5, 0x06a6, 0x06a7, 0x06a8, 0x06a9, 0x06aa, 0x06ab,
    0x06ac, 0x06ad, 0x06ae, 0x06af, 0x06b0, 0x06b1, 0x06b2, 0x06b3,
    0x06b4, 0x06b5, 0x06b6, 0x06b7, 0x06b8, 0x06b9, 0x06ba, 0x06bb,
    0x06bc, 0x06bd, 0x06be, 0x06bf, 0x06c0, 0x06c1, 0x06c2, 0x06c3,
    0x06c4, 0x06c5, 0x06c6, 0x06c7, 0x06c8, 0x06c9, 0x06ca, 0x06cb,
    0x06cc, 0x06cd, 0x06ce, 0x06cf, 0x06d0, 0x06d1, 0x06d2, 0x06d3,
    0x06d4, 0x06d5, 0x06e5, 0x06e6, 0x06ee, 0x06ef, 0x06fa, 0x06fb,
    0x06fc, 0x06fd, 0x06fe, 0x06ff, 0x0700, 0x0701, 0x0702, 0x0703,
    0x0704, 0x0705, 0x0706, 0x0707, 0x0708, 0x0709, 0x070a, 0x070b,
    0x070c, 0x070d, 0x070f, 0x0710, 0x0712, 0x0713, 0x0714, 0x0715,
    0x0716, 0x0717, 0x0718, 0x0719, 0x071a, 0x071b, 0x071c, 0x071d,
    0x071e, 0x071f, 0x0720, 0x0721, 0x0722, 0x0723, 0x0724, 0x0725,
    0x0726, 0x0727, 0x0728, 0x0729, 0x072a, 0x072b, 0x072c, 0x072d,
    0x072e, 0x072f, 0x074d, 0x074e, 0x074f, 0x0750, 0x0751, 0x0752,
    0x0753, 0x0754, 0x0755, 0x0756, 0x0757, 0x0758, 0x0759, 0x075a,
    0x075b, 0x075c, 0x075d, 0x075e, 0x075f, 0x0760, 0x0761, 0x0762,
    0x0763, 0x0764, 0x0765, 0x0766, 0x0767, 0x0768, 0x0769, 0x076a,
    0x076b, 0x076c, 0x076d, 0x076e, 0x076f, 0x0770, 0x0771, 0x0772,
    0x0773, 0x0774, 0x0775, 0x0776, 0x0777, 0x0778, 0x0779, 0x077a,
    0x077b, 0x077c, 0x077d, 0x077e, 0x077f, 0x0780, 0x0781, 0x0782,
    0x0783, 0x0784, 0x0785, 0x0786, 0x0787, 0x0788, 0x0789, 0x078a,
    0x078b, 0x078c, 0x078d, 0x078e, 0x078f, 0x0790, 0x0791, 0x0792,
    0x0793, 0x0794, 0x0795, 0x0796, 0x0797, 0x0798, 0x0799, 0x079a,
    0x079b, 0x079c, 0x079d, 0x079e, 0x079f, 0x07a0, 0x07a1, 0x07a2,
    0x07a3, 0x07a4, 0x07a5, 0x07b1, 0x08a0, 0x08a1, 0x08a2, 0x08a3,
    0x08a4, 0x08a5, 0x08a6, 0x08a7, 0x08a8, 0x08a9, 0x08aa, 0x08ab,
    0x08ac, 0x08ad, 0x08ae, 0x08af, 0x08b0, 0x08b1, 0x08b2, 0x08b3,
    0x08b4, 0x08b6, 0x08b7, 0x08b8, 0x08b9, 0x08ba, 0x08bb, 0x08bc,
    0x08bd, 0xfb50, 0xfb51, 0xfb52, 0xfb53, 0xfb54, 0xfb55, 0xfb56,
    0xfb57, 0xfb58, 0xfb59, 0xfb5a, 0xfb5b, 0xfb5c, 0xfb5d, 0xfb5e,
    0xfb5f, 0xfb60, 0xfb61, 0xfb62, 0xfb63, 0xfb64, 0xfb65, 0xfb66,
    0xfb67, 0xfb68, 0xfb69, 0xfb6a, 0xfb6b, 0xfb6c, 0xfb6d, 0xfb6e,
    0xfb6f, 0xfb70, 0xfb71, 0xfb72, 0xfb73, 0xfb74, 0xfb75, 0xfb76,
    0xfb77, 0xfb78, 0xfb79, 0xfb7a, 0xfb7b, 0xfb7c, 0xfb7d, 0xfb7e,
    0xfb7f, 0xfb80, 0xfb81, 0xfb82, 0xfb83, 0xfb84, 0xfb85, 0xfb86,
    0xfb87, 0xfb88, 0xfb89, 0xfb8a, 0xfb8b, 0xfb8c, 0xfb8d, 0xfb8e,
    0xfb8f, 0xfb90, 0xfb91, 0xfb92, 0xfb93, 0xfb94, 0xfb95, 0xfb96,
    0xfb97, 0xfb98, 0xfb99, 0xfb9a, 0xfb9b, 0xfb9c, 0xfb9d, 0xfb9e,
    0xfb9f, 0xfba0, 0xfba1, 0xfba2, 0xfba3, 0xfba4, 0xfba5, 0xfba6,
    0xfba7, 0xfba8, 0xfba9, 0xfbaa, 0xfbab, 0xfbac, 0xfbad, 0xfbae,
    0xfbaf, 0xfbb0, 0xfbb1, 0xfbb2, 0xfbb3, 0xfbb4, 0xfbb5, 0xfbb6,
    0xfbb7, 0xfbb8, 0xfbb9, 0xfbba, 0xfbbb, 0xfbbc, 0xfbbd, 0xfbbe,
    0xfbbf, 0xfbc0, 0xfbc1, 0xfbd3, 0xfbd4, 0xfbd5, 0xfbd6, 0xfbd7,
    0xfbd8, 0xfbd9, 0xfbda, 0xfbdb, 0xfbdc, 0xfbdd, 0xfbde, 0xfbdf,
    0xfbe0, 0xfbe1, 0xfbe2, 0xfbe3, 0xfbe4, 0xfbe5, 0xfbe6, 0xfbe7,
    0xfbe8, 0xfbe9, 0xfbea, 0xfbeb, 0xfbec, 0xfbed, 0xfbee, 0xfbef,
    0xfbf0, 0xfbf1, 0xfbf2, 0xfbf3, 0xfbf4, 0xfbf5, 0xfbf6, 0xfbf7,
    0xfbf8, 0xfbf9, 0xfbfa, 0xfbfb, 0xfbfc, 0xfbfd, 0xfbfe, 0xfbff,
    0xfc00, 0xfc01, 0xfc02, 0xfc03, 0xfc04, 0xfc05, 0xfc06, 0xfc07,
    0xfc08, 0xfc09, 0xfc0a, 0xfc0b, 0xfc0c, 0xfc0d, 0xfc0e, 0xfc0f,
    0xfc10, 0xfc11, 0xfc12, 0xfc13, 0xfc14, 0xfc15, 0xfc16, 0xfc17,
    0xfc18, 0xfc19, 0xfc1a, 0xfc1b, 0xfc1c, 0xfc1d, 0xfc1e, 0xfc1f,
    0xfc20, 0xfc21, 0xfc22, 0xfc23, 0xfc24, 0xfc25, 0xfc26, 0xfc27,
    0xfc28, 0xfc29, 0xfc2a, 0xfc2b, 0xfc2c, 0xfc2d, 0xfc2e, 0xfc2f,
    0xfc30, 0xfc31, 0xfc32, 0xfc33, 0xfc34, 0xfc35, 0xfc36, 0xfc37,
    0xfc38, 0xfc39, 0xfc3a, 0xfc3b, 0xfc3c, 0xfc3d, 0xfc3e, 0xfc3f,
    0xfc40, 0xfc41, 0xfc42, 0xfc43, 0xfc44, 0xfc45, 0xfc46, 0xfc47,
    0xfc48, 0xfc49, 0xfc4a, 0xfc4b, 0xfc4c, 0xfc4d, 0xfc4e, 0xfc4f,
    0xfc50, 0xfc51, 0xfc52, 0xfc53, 0xfc54, 0xfc55, 0xfc56, 0xfc57,
    0xfc58, 0xfc59, 0xfc5a, 0xfc5b, 0xfc5c, 0xfc5d, 0xfc5e, 0xfc5f,
    0xfc60, 0xfc61, 0xfc62, 0xfc63, 0xfc64, 0xfc65, 0xfc66, 0xfc67,
    0xfc68, 0xfc69, 0xfc6a, 0xfc6b, 0xfc6c, 0xfc6d, 0xfc6e, 0xfc6f,
    0xfc70, 0xfc71, 0xfc72, 0xfc73, 0xfc74, 0xfc75, 0xfc76, 0xfc77,
    0xfc78, 0xfc79, 0xfc7a, 0xfc7b, 0xfc7c, 0xfc7d, 0xfc7e, 0xfc7f,
    0xfc80, 0xfc81, 0xfc82, 0xfc83, 0xfc84, 0xfc85, 0xfc86, 0xfc87,
    0xfc88, 0xfc89, 0xfc8a, 0xfc8b, 0xfc8c, 0xfc8d, 0xfc8e, 0xfc8f,
    0xfc90, 0xfc91, 0xfc92, 0xfc93, 0xfc94, 0xfc95, 0xfc96, 0xfc97,
    0xfc98, 0xfc99, 0xfc9a, 0xfc9b, 0xfc9c, 0xfc9d, 0xfc9e, 0xfc9f,
    0xfca0, 0xfca1, 0xfca2, 0xfca3, 0xfca4, 0xfca5, 0xfca6, 0xfca7,
    0xfca8, 0xfca9, 0xfcaa, 0xfcab, 0xfcac, 0xfcad, 0xfcae, 0xfcaf,
    0xfcb0, 0xfcb1, 0xfcb2, 0xfcb3, 0xfcb4, 0xfcb5, 0xfcb6, 0xfcb7,
    0xfcb8, 0xfcb9, 0xfcba, 0xfcbb, 0xfcbc, 0xfcbd, 0xfcbe, 0xfcbf,
    0xfcc0, 0xfcc1, 0xfcc2, 0xfcc3, 0xfcc4, 0xfcc5, 0xfcc6, 0xfcc7,
    0xfcc8, 0xfcc9, 0xfcca, 0xfccb, 0xfccc, 0xfccd, 0xfcce, 0xfccf,
    0xfcd0, 0xfcd1, 0xfcd2, 0xfcd3, 0xfcd4, 0xfcd5, 0xfcd6, 0xfcd7,
    0xfcd8, 0xfcd9, 0xfcda, 0xfcdb, 0xfcdc, 0xfcdd, 0xfcde, 0xfcdf,
    0xfce0, 0xfce1, 0xfce2, 0xfce3, 0xfce4, 0xfce5, 0xfce6, 0xfce7,
    0xfce8, 0xfce9, 0xfcea, 0xfceb, 0xfcec, 0xfced, 0xfcee, 0xfcef,
    0xfcf0, 0xfcf1, 0xfcf2, 0xfcf3, 0xfcf4, 0xfcf5, 0xfcf6, 0xfcf7,
    0xfcf8, 0xfcf9, 0xfcfa, 0xfcfb, 0xfcfc, 0xfcfd, 0xfcfe, 0xfcff,
    0xfd00, 0xfd01, 0xfd02, 0xfd03, 0xfd04, 0xfd05, 0xfd06, 0xfd07,
    0xfd08, 0xfd09, 0xfd0a, 0xfd0b, 0xfd0c, 0xfd0d, 0xfd0e, 0xfd0f,
    0xfd10, 0xfd11, 0xfd12, 0xfd13, 0xfd14, 0xfd15, 0xfd16, 0xfd17,
    0xfd18, 0xfd19, 0xfd1a, 0xfd1b, 0xfd1c, 0xfd1d, 0xfd1e, 0xfd1f,
    0xfd20, 0xfd21, 0xfd22, 0xfd23, 0xfd24, 0xfd25, 0xfd26, 0xfd27,
    0xfd28, 0xfd29, 0xfd2a, 0xfd2b, 0xfd2c, 0xfd2d, 0xfd2e, 0xfd2f,
    0xfd30, 0xfd31, 0xfd32, 0xfd33, 0xfd34, 0xfd35, 0xfd36, 0xfd37,
    0xfd38, 0xfd39, 0xfd3a, 0xfd3b, 0xfd3c, 0xfd3d, 0xfd50, 0xfd51,
    0xfd52, 0xfd53, 0xfd54, 0xfd55, 0xfd56, 0xfd57, 0xfd58, 0xfd59,
    0xfd5a, 0xfd5b, 0xfd5c, 0xfd5d, 0xfd5e, 0xfd5f, 0xfd60, 0xfd61,
    0xfd62, 0xfd63, 0xfd64, 0xfd65, 0xfd66, 0xfd67, 0xfd68, 0xfd69,
    0xfd6a, 0xfd6b, 0xfd6c, 0xfd6d, 0xfd6e, 0xfd6f, 0xfd70, 0xfd71,
    0xfd72, 0xfd73, 0xfd74, 0xfd75, 0xfd76, 0xfd77, 0xfd78, 0xfd79,
    0xfd7a, 0xfd7b, 0xfd7c, 0xfd7d, 0xfd7e, 0xfd7f, 0xfd80, 0xfd81,
    0xfd82, 0xfd83, 0xfd84, 0xfd85, 0xfd86, 0xfd87, 0xfd88, 0xfd89,
    0xfd8a, 0xfd8b, 0xfd8c, 0xfd8d, 0xfd8e, 0xfd8f, 0xfd92, 0xfd93,
    0xfd94, 0xfd95, 0xfd96, 0xfd97, 0xfd98, 0xfd99, 0xfd9a, 0xfd9b,
    0xfd9c, 0xfd9d, 0xfd9e, 0xfd9f, 0xfda0, 0xfda1, 0xfda2, 0xfda3,
    0xfda4, 0xfda5, 0xfda6, 0xfda7, 0xfda8, 0xfda9, 0xfdaa, 0xfdab,
    0xfdac, 0xfdad, 0xfdae, 0xfdaf, 0xfdb0, 0xfdb1, 0xfdb2, 0xfdb3,
    0xfdb4, 0xfdb5, 0xfdb6, 0xfdb7, 0xfdb8, 0xfdb9, 0xfdba, 0xfdbb,
    0xfdbc, 0xfdbd, 0xfdbe, 0xfdbf, 0xfdc0, 0xfdc1, 0xfdc2, 0xfdc3,
    0xfdc4, 0xfdc5, 0xfdc6, 0xfdc7, 0xfdf0, 0xfdf1, 0xfdf2, 0xfdf3,
    0xfdf4, 0xfdf5, 0xfdf6, 0xfdf7, 0xfdf8, 0xfdf9, 0xfdfa, 0xfdfb,
    0xfdfc, 0xfe70, 0xfe71, 0xfe72, 0xfe73, 0xfe74, 0xfe76, 0xfe77,
    0xfe78, 0xfe79, 0xfe7a, 0xfe7b, 0xfe7c, 0xfe7d, 0xfe7e, 0xfe7f,
    0xfe80, 0xfe81, 0xfe82, 0xfe83, 0xfe84, 0xfe85, 0xfe86, 0xfe87,
    0xfe88, 0xfe89, 0xfe8a, 0xfe8b, 0xfe8c, 0xfe8d, 0xfe8e, 0xfe8f,
    0xfe90, 0xfe91, 0xfe92, 0xfe93, 0xfe94, 0xfe95, 0xfe96, 0xfe97,
    0xfe98, 0xfe99, 0xfe9a, 0xfe9b, 0xfe9c, 0xfe9d, 0xfe9e, 0xfe9f,
    0xfea0, 0xfea1, 0xfea2, 0xfea3, 0xfea4, 0xfea5, 0xfea6, 0xfea7,
    0xfea8, 0xfea9, 0xfeaa, 0xfeab, 0xfeac, 0xfead, 0xfeae, 0xfeaf,
    0xfeb0, 0xfeb1, 0xfeb2, 0xfeb3, 0xfeb4, 0xfeb5, 0xfeb6, 0xfeb7,
    0xfeb8, 0xfeb9, 0xfeba, 0xfebb, 0xfebc, 0xfebd, 0xfebe, 0xfebf,
    0xfec0, 0xfec1, 0xfec2, 0xfec3, 0xfec4, 0xfec5, 0xfec6, 0xfec7,
    0xfec8, 0xfec9, 0xfeca, 0xfecb, 0xfecc, 0xfecd, 0xfece, 0xfecf,
    0xfed0, 0xfed1, 0xfed2, 0xfed3, 0xfed4, 0xfed5, 0xfed6, 0xfed7,
    0xfed8, 0xfed9, 0xfeda, 0xfedb, 0xfedc, 0xfedd, 0xfede, 0xfedf,
    0xfee0, 0xfee1, 0xfee2, 0xfee3, 0xfee4, 0xfee5, 0xfee6, 0xfee7,
    0xfee8, 0xfee9, 0xfeea, 0xfeeb, 0xfeec, 0xfeed, 0xfeee, 0xfeef,
    0xfef0, 0xfef1, 0xfef2, 0xfef3, 0xfef4, 0xfef5, 0xfef6, 0xfef7,
    0xfef8, 0xfef9, 0xfefa, 0xfefb, 0xfefc, 0x1ee00, 0x1ee01, 0x1ee02,
    0x1ee03, 0x1ee05, 0x1ee06, 0x1ee07, 0x1ee08, 0x1ee09, 0x1ee0a,
    0x1ee0b, 0x1ee0c, 0x1ee0d, 0x1ee0e, 0x1ee0f, 0x1ee10, 0x1ee11,
    0x1ee12, 0x1ee13, 0x1ee14, 0x1ee15, 0x1ee16, 0x1ee17, 0x1ee18,
    0x1ee19, 0x1ee1a, 0x1ee1b, 0x1ee1c, 0x1ee1d, 0x1ee1e, 0x1ee1f,
    0x1ee21, 0x1ee22, 0x1ee24, 0x1ee27, 0x1ee29, 0x1ee2a, 0x1ee2b,
    0x1ee2c, 0x1ee2d, 0x1ee2e, 0x1ee2f, 0x1ee30, 0x1ee31, 0x1ee32,
    0x1ee34, 0x1ee35, 0x1ee36, 0x1ee37, 0x1ee39, 0x1ee3b, 0x1ee42,
    0x1ee47, 0x1ee49, 0x1ee4b, 0x1ee4d, 0x1ee4e, 0x1ee4f, 0x1ee51,
    0x1ee52, 0x1ee54, 0x1ee57, 0x1ee59, 0x1ee5b, 0x1ee5d, 0x1ee5f,
    0x1ee61, 0x1ee62, 0x1ee64, 0x1ee67, 0x1ee68, 0x1ee69, 0x1ee6a,
    0x1ee6c, 0x1ee6d, 0x1ee6e, 0x1ee6f, 0x1ee70, 0x1ee71, 0x1ee72,
    0x1ee74, 0x1ee75, 0x1ee76, 0x1ee77, 0x1ee79, 0x1ee7a, 0x1ee7b,
    0x1ee7c, 0x1ee7e, 0x1ee80, 0x1ee81, 0x1ee82, 0x1ee83, 0x1ee84,
    0x1ee85, 0x1ee86, 0x1ee87, 0x1ee88, 0x1ee89, 0x1ee8b, 0x1ee8c,
    0x1ee8d, 0x1ee8e, 0x1ee8f, 0x1ee90, 0x1ee91, 0x1ee92, 0x1ee93,
    0x1ee94, 0x1ee95, 0x1ee96, 0x1ee97, 0x1ee98, 0x1ee99, 0x1ee9a,
    0x1ee9b, 0x1eea1, 0x1eea2, 0x1eea3, 0x1eea5, 0x1eea6, 0x1eea7,
    0x1eea8, 0x1eea9, 0x1eeab, 0x1eeac, 0x1eead, 0x1eeae, 0x1eeaf,
    0x1eeb0, 0x1eeb1, 0x1eeb2, 0x1eeb3, 0x1eeb4, 0x1eeb5, 0x1eeb6,
    0x1eeb7, 0x1eeb8, 0x1eeb9, 0x1eeba, 0x1eebb
};

/* Class AN:  Arabic numbers */
std::unordered_set<char32_t> AN
{
    0x0600, 0x0601, 0x0602, 0x0603, 0x0604, 0x0605, 0x0660, 0x0661,
    0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667, 0x0668, 0x0669,
    0x066b, 0x066c, 0x06dd, 0x08e2, 0x10e60, 0x10e61, 0x10e62,
    0x10e63, 0x10e64, 0x10e65, 0x10e66, 0x10e67, 0x10e68, 0x10e69,
    0x10e6a, 0x10e6b, 0x10e6c, 0x10e6d, 0x10e6e, 0x10e6f, 0x10e70,
    0x10e71, 0x10e72, 0x10e73, 0x10e74, 0x10e75, 0x10e76, 0x10e77,
    0x10e78, 0x10e79, 0x10e7a, 0x10e7b, 0x10e7c, 0x10e7d, 0x10e7e
};

/* Class B:  Paragraph separators */
std::unordered_set<char32_t> B
{
    0x0a, 0x0d, 0x1c, 0x1d, 0x1e, 0x85, 0x2029
};

/* Class BN:  Boundary Neutrals */
std::unordered_set<char32_t> BN
{
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
    0x0008, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014,
    0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001a, 0x001b, 0x007f,
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0086, 0x0087, 0x0088,
    0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f, 0x0090,
    0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098,
    0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f, 0x00ad,
    0x180e, 0x200b, 0x200c, 0x200d, 0x2060, 0x2061, 0x2062, 0x2063,
    0x2064, 0x206a, 0x206b, 0x206c, 0x206d, 0x206e, 0x206f, 0xfeff,
    0x1bca0, 0x1bca1, 0x1bca2, 0x1bca3, 0x1d173, 0x1d174, 0x1d175,
    0x1d176, 0x1d177, 0x1d178, 0x1d179, 0x1d17a, 0xe0001, 0xe0020,
    0xe0021, 0xe0022, 0xe0023, 0xe0024, 0xe0025, 0xe0026, 0xe0027,
    0xe0028, 0xe0029, 0xe002a, 0xe002b, 0xe002c, 0xe002d, 0xe002e,
    0xe002f, 0xe0030, 0xe0031, 0xe0032, 0xe0033, 0xe0034, 0xe0035,
    0xe0036, 0xe0037, 0xe0038, 0xe0039, 0xe003a, 0xe003b, 0xe003c,
    0xe003d, 0xe003e, 0xe003f, 0xe0040, 0xe0041, 0xe0042, 0xe0043,
    0xe0044, 0xe0045, 0xe0046, 0xe0047, 0xe0048, 0xe0049, 0xe004a,
    0xe004b, 0xe004c, 0xe004d, 0xe004e, 0xe004f, 0xe0050, 0xe0051,
    0xe0052, 0xe0053, 0xe0054, 0xe0055, 0xe0056, 0xe0057, 0xe0058,
    0xe0059, 0xe005a, 0xe005b, 0xe005c, 0xe005d, 0xe005e, 0xe005f,
    0xe0060, 0xe0061, 0xe0062, 0xe0063, 0xe0064, 0xe0065, 0xe0066,
    0xe0067, 0xe0068, 0xe0069, 0xe006a, 0xe006b, 0xe006c, 0xe006d,
    0xe006e, 0xe006f, 0xe0070, 0xe0071, 0xe0072, 0xe0073, 0xe0074,
    0xe0075, 0xe0076, 0xe0077, 0xe0078, 0xe0079, 0xe007a, 0xe007b,
    0xe007c, 0xe007d, 0xe007e, 0xe007f
};

/* Class CS:  Common separators */
std::unordered_set<char32_t> CS
{
    0x002c, 0x002e, 0x002f, 0x003a, 0x00a0, 0x060c, 0x202f, 0x2044,
    0xfe50, 0xfe52, 0xfe55, 0xff0c, 0xff0e, 0xff0f, 0xff1a
};

/* Class EN:  European numbers */
std::unordered_set<char32_t> EN
{
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x00b2, 0x00b3, 0x00b9, 0x06f0, 0x06f1, 0x06f2,
    0x06f3, 0x06f4, 0x06f5, 0x06f6, 0x06f7, 0x06f8, 0x06f9, 0x2070,
    0x2074, 0x2075, 0x2076, 0x2077, 0x2078, 0x2079, 0x2080, 0x2081,
    0x2082, 0x2083, 0x2084, 0x2085, 0x2086, 0x2087, 0x2088, 0x2089,
    0x2488, 0x2489, 0x248a, 0x248b, 0x248c, 0x248d, 0x248e, 0x248f,
    0x2490, 0x2491, 0x2492, 0x2493, 0x2494, 0x2495, 0x2496, 0x2497,
    0x2498, 0x2499, 0x249a, 0x249b, 0xff10, 0xff11, 0xff12, 0xff13,
    0xff14, 0xff15, 0xff16, 0xff17, 0xff18, 0xff19, 0x102e1, 0x102e2,
    0x102e3, 0x102e4, 0x102e5, 0x102e6, 0x102e7, 0x102e8, 0x102e9,
    0x102ea, 0x102eb, 0x102ec, 0x102ed, 0x102ee, 0x102ef, 0x102f0,
    0x102f1, 0x102f2, 0x102f3, 0x102f4, 0x102f5, 0x102f6, 0x102f7,
    0x102f8, 0x102f9, 0x102fa, 0x102fb, 0x1d7ce, 0x1d7cf, 0x1d7d0,
    0x1d7d1, 0x1d7d2, 0x1d7d3, 0x1d7d4, 0x1d7d5, 0x1d7d6, 0x1d7d7,
    0x1d7d8, 0x1d7d9, 0x1d7da, 0x1d7db, 0x1d7dc, 0x1d7dd, 0x1d7de,
    0x1d7df, 0x1d7e0, 0x1d7e1, 0x1d7e2, 0x1d7e3, 0x1d7e4, 0x1d7e5,
    0x1d7e6, 0x1d7e7, 0x1d7e8, 0x1d7e9, 0x1d7ea, 0x1d7eb, 0x1d7ec,
    0x1d7ed, 0x1d7ee, 0x1d7ef, 0x1d7f0, 0x1d7f1, 0x1d7f2, 0x1d7f3,
    0x1d7f4, 0x1d7f5, 0x1d7f6, 0x1d7f7, 0x1d7f8, 0x1d7f9, 0x1d7fa,
    0x1d7fb, 0x1d7fc, 0x1d7fd, 0x1d7fe, 0x1d7ff, 0x1f100, 0x1f101,
    0x1f102, 0x1f103, 0x1f104, 0x1f105, 0x1f106, 0x1f107, 0x1f108,
    0x1f109, 0x1f10a
};

/* Class R:  Right-to-left */
std::unordered_set<char32_t> R
{
    0x05be, 0x05c0, 0x05c3, 0x05c6, 0x05d0, 0x05d1, 0x05d2, 0x05d3,
    0x05d4, 0x05d5, 0x05d6, 0x05d7, 0x05d8, 0x05d9, 0x05da, 0x05db,
    0x05dc, 0x05dd, 0x05de, 0x05df, 0x05e0, 0x05e1, 0x05e2, 0x05e3,
    0x05e4, 0x05e5, 0x05e6, 0x05e7, 0x05e8, 0x05e9, 0x05ea, 0x05f0,
    0x05f1, 0x05f2, 0x05f3, 0x05f4, 0x07c0, 0x07c1, 0x07c2, 0x07c3,
    0x07c4, 0x07c5, 0x07c6, 0x07c7, 0x07c8, 0x07c9, 0x07ca, 0x07cb,
    0x07cc, 0x07cd, 0x07ce, 0x07cf, 0x07d0, 0x07d1, 0x07d2, 0x07d3,
    0x07d4, 0x07d5, 0x07d6, 0x07d7, 0x07d8, 0x07d9, 0x07da, 0x07db,
    0x07dc, 0x07dd, 0x07de, 0x07df, 0x07e0, 0x07e1, 0x07e2, 0x07e3,
    0x07e4, 0x07e5, 0x07e6, 0x07e7, 0x07e8, 0x07e9, 0x07ea, 0x07f4,
    0x07f5, 0x07fa, 0x0800, 0x0801, 0x0802, 0x0803, 0x0804, 0x0805,
    0x0806, 0x0807, 0x0808, 0x0809, 0x080a, 0x080b, 0x080c, 0x080d,
    0x080e, 0x080f, 0x0810, 0x0811, 0x0812, 0x0813, 0x0814, 0x0815,
    0x081a, 0x0824, 0x0828, 0x0830, 0x0831, 0x0832, 0x0833, 0x0834,
    0x0835, 0x0836, 0x0837, 0x0838, 0x0839, 0x083a, 0x083b, 0x083c,
    0x083d, 0x083e, 0x0840, 0x0841, 0x0842, 0x0843, 0x0844, 0x0845,
    0x0846, 0x0847, 0x0848, 0x0849, 0x084a, 0x084b, 0x084c, 0x084d,
    0x084e, 0x084f, 0x0850, 0x0851, 0x0852, 0x0853, 0x0854, 0x0855,
    0x0856, 0x0857, 0x0858, 0x085e, 0x200f, 0xfb1d, 0xfb1f, 0xfb20,
    0xfb21, 0xfb22, 0xfb23, 0xfb24, 0xfb25, 0xfb26, 0xfb27, 0xfb28,
    0xfb2a, 0xfb2b, 0xfb2c, 0xfb2d, 0xfb2e, 0xfb2f, 0xfb30, 0xfb31,
    0xfb32, 0xfb33, 0xfb34, 0xfb35, 0xfb36, 0xfb38, 0xfb39, 0xfb3a,
    0xfb3b, 0xfb3c, 0xfb3e, 0xfb40, 0xfb41, 0xfb43, 0xfb44, 0xfb46,
    0xfb47, 0xfb48, 0xfb49, 0xfb4a, 0xfb4b, 0xfb4c, 0xfb4d, 0xfb4e,
    0xfb4f, 0x10800, 0x10801, 0x10802, 0x10803, 0x10804, 0x10805,
    0x10808, 0x1080a, 0x1080b, 0x1080c, 0x1080d, 0x1080e, 0x1080f,
    0x10810, 0x10811, 0x10812, 0x10813, 0x10814, 0x10815, 0x10816,
    0x10817, 0x10818, 0x10819, 0x1081a, 0x1081b, 0x1081c, 0x1081d,
    0x1081e, 0x1081f, 0x10820, 0x10821, 0x10822, 0x10823, 0x10824,
    0x10825, 0x10826, 0x10827, 0x10828, 0x10829, 0x1082a, 0x1082b,
    0x1082c, 0x1082d, 0x1082e, 0x1082f, 0x10830, 0x10831, 0x10832,
    0x10833, 0x10834, 0x10835, 0x10837, 0x10838, 0x1083c, 0x1083f,
    0x10840, 0x10841, 0x10842, 0x10843, 0x10844, 0x10845, 0x10846,
    0x10847, 0x10848, 0x10849, 0x1084a, 0x1084b, 0x1084c, 0x1084d,
    0x1084e, 0x1084f, 0x10850, 0x10851, 0x10852, 0x10853, 0x10854,
    0x10855, 0x10857, 0x10858, 0x10859, 0x1085a, 0x1085b, 0x1085c,
    0x1085d, 0x1085e, 0x1085f, 0x10860, 0x10861, 0x10862, 0x10863,
    0x10864, 0x10865, 0x10866, 0x10867, 0x10868, 0x10869, 0x1086a,
    0x1086b, 0x1086c, 0x1086d, 0x1086e, 0x1086f, 0x10870, 0x10871,
    0x10872, 0x10873, 0x10874, 0x10875, 0x10876, 0x10877, 0x10878,
    0x10879, 0x1087a, 0x1087b, 0x1087c, 0x1087d, 0x1087e, 0x1087f,
    0x10880, 0x10881, 0x10882, 0x10883, 0x10884, 0x10885, 0x10886,
    0x10887, 0x10888, 0x10889, 0x1088a, 0x1088b, 0x1088c, 0x1088d,
    0x1088e, 0x1088f, 0x10890, 0x10891, 0x10892, 0x10893, 0x10894,
    0x10895, 0x10896, 0x10897, 0x10898, 0x10899, 0x1089a, 0x1089b,
    0x1089c, 0x1089d, 0x1089e, 0x108a7, 0x108a8, 0x108a9, 0x108aa,
    0x108ab, 0x108ac, 0x108ad, 0x108ae, 0x108af, 0x108e0, 0x108e1,
    0x108e2, 0x108e3, 0x108e4, 0x108e5, 0x108e6, 0x108e7, 0x108e8,
    0x108e9, 0x108ea, 0x108eb, 0x108ec, 0x108ed, 0x108ee, 0x108ef,
    0x108f0, 0x108f1, 0x108f2, 0x108f4, 0x108f5, 0x108fb, 0x108fc,
    0x108fd, 0x108fe, 0x108ff, 0x10900, 0x10901, 0x10902, 0x10903,
    0x10904, 0x10905, 0x10906, 0x10907, 0x10908, 0x10909, 0x1090a,
    0x1090b, 0x1090c, 0x1090d, 0x1090e, 0x1090f, 0x10910, 0x10911,
    0x10912, 0x10913, 0x10914, 0x10915, 0x10916, 0x10917, 0x10918,
    0x10919, 0x1091a, 0x1091b, 0x10920, 0x10921, 0x10922, 0x10923,
    0x10924, 0x10925, 0x10926, 0x10927, 0x10928, 0x10929, 0x1092a,
    0x1092b, 0x1092c, 0x1092d, 0x1092e, 0x1092f, 0x10930, 0x10931,
    0x10932, 0x10933, 0x10934, 0x10935, 0x10936, 0x10937, 0x10938,
    0x10939, 0x1093f, 0x10980, 0x10981, 0x10982, 0x10983, 0x10984,
    0x10985, 0x10986, 0x10987, 0x10988, 0x10989, 0x1098a, 0x1098b,
    0x1098c, 0x1098d, 0x1098e, 0x1098f, 0x10990, 0x10991, 0x10992,
    0x10993, 0x10994, 0x10995, 0x10996, 0x10997, 0x10998, 0x10999,
    0x1099a, 0x1099b, 0x1099c, 0x1099d, 0x1099e, 0x1099f, 0x109a0,
    0x109a1, 0x109a2, 0x109a3, 0x109a4, 0x109a5, 0x109a6, 0x109a7,
    0x109a8, 0x109a9, 0x109aa, 0x109ab, 0x109ac, 0x109ad, 0x109ae,
    0x109af, 0x109b0, 0x109b1, 0x109b2, 0x109b3, 0x109b4, 0x109b5,
    0x109b6, 0x109b7, 0x109bc, 0x109bd, 0x109be, 0x109bf, 0x109c0,
    0x109c1, 0x109c2, 0x109c3, 0x109c4, 0x109c5, 0x109c6, 0x109c7,
    0x109c8, 0x109c9, 0x109ca, 0x109cb, 0x109cc, 0x109cd, 0x109ce,
    0x109cf, 0x109d2, 0x109d3, 0x109d4, 0x109d5, 0x109d6, 0x109d7,
    0x109d8, 0x109d9, 0x109da, 0x109db, 0x109dc, 0x109dd, 0x109de,
    0x109df, 0x109e0, 0x109e1, 0x109e2, 0x109e3, 0x109e4, 0x109e5,
    0x109e6, 0x109e7, 0x109e8, 0x109e9, 0x109ea, 0x109eb, 0x109ec,
    0x109ed, 0x109ee, 0x109ef, 0x109f0, 0x109f1, 0x109f2, 0x109f3,
    0x109f4, 0x109f5, 0x109f6, 0x109f7, 0x109f8, 0x109f9, 0x109fa,
    0x109fb, 0x109fc, 0x109fd, 0x109fe, 0x109ff, 0x10a00, 0x10a10,
    0x10a11, 0x10a12, 0x10a13, 0x10a15, 0x10a16, 0x10a17, 0x10a19,
    0x10a1a, 0x10a1b, 0x10a1c, 0x10a1d, 0x10a1e, 0x10a1f, 0x10a20,
    0x10a21, 0x10a22, 0x10a23, 0x10a24, 0x10a25, 0x10a26, 0x10a27,
    0x10a28, 0x10a29, 0x10a2a, 0x10a2b, 0x10a2c, 0x10a2d, 0x10a2e,
    0x10a2f, 0x10a30, 0x10a31, 0x10a32, 0x10a33, 0x10a40, 0x10a41,
    0x10a42, 0x10a43, 0x10a44, 0x10a45, 0x10a46, 0x10a47, 0x10a50,
    0x10a51, 0x10a52, 0x10a53, 0x10a54, 0x10a55, 0x10a56, 0x10a57,
    0x10a58, 0x10a60, 0x10a61, 0x10a62, 0x10a63, 0x10a64, 0x10a65,
    0x10a66, 0x10a67, 0x10a68, 0x10a69, 0x10a6a, 0x10a6b, 0x10a6c,
    0x10a6d, 0x10a6e, 0x10a6f, 0x10a70, 0x10a71, 0x10a72, 0x10a73,
    0x10a74, 0x10a75, 0x10a76, 0x10a77, 0x10a78, 0x10a79, 0x10a7a,
    0x10a7b, 0x10a7c, 0x10a7d, 0x10a7e, 0x10a7f, 0x10a80, 0x10a81,
    0x10a82, 0x10a83, 0x10a84, 0x10a85, 0x10a86, 0x10a87, 0x10a88,
    0x10a89, 0x10a8a, 0x10a8b, 0x10a8c, 0x10a8d, 0x10a8e, 0x10a8f,
    0x10a90, 0x10a91, 0x10a92, 0x10a93, 0x10a94, 0x10a95, 0x10a96,
    0x10a97, 0x10a98, 0x10a99, 0x10a9a, 0x10a9b, 0x10a9c, 0x10a9d,
    0x10a9e, 0x10a9f, 0x10ac0, 0x10ac1, 0x10ac2, 0x10ac3, 0x10ac4,
    0x10ac5, 0x10ac6, 0x10ac7, 0x10ac8, 0x10ac9, 0x10aca, 0x10acb,
    0x10acc, 0x10acd, 0x10ace, 0x10acf, 0x10ad0, 0x10ad1, 0x10ad2,
    0x10ad3, 0x10ad4, 0x10ad5, 0x10ad6, 0x10ad7, 0x10ad8, 0x10ad9,
    0x10ada, 0x10adb, 0x10adc, 0x10add, 0x10ade, 0x10adf, 0x10ae0,
    0x10ae1, 0x10ae2, 0x10ae3, 0x10ae4, 0x10aeb, 0x10aec, 0x10aed,
    0x10aee, 0x10aef, 0x10af0, 0x10af1, 0x10af2, 0x10af3, 0x10af4,
    0x10af5, 0x10af6, 0x10b00, 0x10b01, 0x10b02, 0x10b03, 0x10b04,
    0x10b05, 0x10b06, 0x10b07, 0x10b08, 0x10b09, 0x10b0a, 0x10b0b,
    0x10b0c, 0x10b0d, 0x10b0e, 0x10b0f, 0x10b10, 0x10b11, 0x10b12,
    0x10b13, 0x10b14, 0x10b15, 0x10b16, 0x10b17, 0x10b18, 0x10b19,
    0x10b1a, 0x10b1b, 0x10b1c, 0x10b1d, 0x10b1e, 0x10b1f, 0x10b20,
    0x10b21, 0x10b22, 0x10b23, 0x10b24, 0x10b25, 0x10b26, 0x10b27,
    0x10b28, 0x10b29, 0x10b2a, 0x10b2b, 0x10b2c, 0x10b2d, 0x10b2e,
    0x10b2f, 0x10b30, 0x10b31, 0x10b32, 0x10b33, 0x10b34, 0x10b35,
    0x10b40, 0x10b41, 0x10b42, 0x10b43, 0x10b44, 0x10b45, 0x10b46,
    0x10b47, 0x10b48, 0x10b49, 0x10b4a, 0x10b4b, 0x10b4c, 0x10b4d,
    0x10b4e, 0x10b4f, 0x10b50, 0x10b51, 0x10b52, 0x10b53, 0x10b54,
    0x10b55, 0x10b58, 0x10b59, 0x10b5a, 0x10b5b, 0x10b5c, 0x10b5d,
    0x10b5e, 0x10b5f, 0x10b60, 0x10b61, 0x10b62, 0x10b63, 0x10b64,
    0x10b65, 0x10b66, 0x10b67, 0x10b68, 0x10b69, 0x10b6a, 0x10b6b,
    0x10b6c, 0x10b6d, 0x10b6e, 0x10b6f, 0x10b70, 0x10b71, 0x10b72,
    0x10b78, 0x10b79, 0x10b7a, 0x10b7b, 0x10b7c, 0x10b7d, 0x10b7e,
    0x10b7f, 0x10b80, 0x10b81, 0x10b82, 0x10b83, 0x10b84, 0x10b85,
    0x10b86, 0x10b87, 0x10b88, 0x10b89, 0x10b8a, 0x10b8b, 0x10b8c,
    0x10b8d, 0x10b8e, 0x10b8f, 0x10b90, 0x10b91, 0x10b99, 0x10b9a,
    0x10b9b, 0x10b9c, 0x10ba9, 0x10baa, 0x10bab, 0x10bac, 0x10bad,
    0x10bae, 0x10baf, 0x10c00, 0x10c01, 0x10c02, 0x10c03, 0x10c04,
    0x10c05, 0x10c06, 0x10c07, 0x10c08, 0x10c09, 0x10c0a, 0x10c0b,
    0x10c0c, 0x10c0d, 0x10c0e, 0x10c0f, 0x10c10, 0x10c11, 0x10c12,
    0x10c13, 0x10c14, 0x10c15, 0x10c16, 0x10c17, 0x10c18, 0x10c19,
    0x10c1a, 0x10c1b, 0x10c1c, 0x10c1d, 0x10c1e, 0x10c1f, 0x10c20,
    0x10c21, 0x10c22, 0x10c23, 0x10c24, 0x10c25, 0x10c26, 0x10c27,
    0x10c28, 0x10c29, 0x10c2a, 0x10c2b, 0x10c2c, 0x10c2d, 0x10c2e,
    0x10c2f, 0x10c30, 0x10c31, 0x10c32, 0x10c33, 0x10c34, 0x10c35,
    0x10c36, 0x10c37, 0x10c38, 0x10c39, 0x10c3a, 0x10c3b, 0x10c3c,
    0x10c3d, 0x10c3e, 0x10c3f, 0x10c40, 0x10c41, 0x10c42, 0x10c43,
    0x10c44, 0x10c45, 0x10c46, 0x10c47, 0x10c48, 0x10c80, 0x10c81,
    0x10c82, 0x10c83, 0x10c84, 0x10c85, 0x10c86, 0x10c87, 0x10c88,
    0x10c89, 0x10c8a, 0x10c8b, 0x10c8c, 0x10c8d, 0x10c8e, 0x10c8f,
    0x10c90, 0x10c91, 0x10c92, 0x10c93, 0x10c94, 0x10c95, 0x10c96,
    0x10c97, 0x10c98, 0x10c99, 0x10c9a, 0x10c9b, 0x10c9c, 0x10c9d,
    0x10c9e, 0x10c9f, 0x10ca0, 0x10ca1, 0x10ca2, 0x10ca3, 0x10ca4,
    0x10ca5, 0x10ca6, 0x10ca7, 0x10ca8, 0x10ca9, 0x10caa, 0x10cab,
    0x10cac, 0x10cad, 0x10cae, 0x10caf, 0x10cb0, 0x10cb1, 0x10cb2,
    0x10cc0, 0x10cc1, 0x10cc2, 0x10cc3, 0x10cc4, 0x10cc5, 0x10cc6,
    0x10cc7, 0x10cc8, 0x10cc9, 0x10cca, 0x10ccb, 0x10ccc, 0x10ccd,
    0x10cce, 0x10ccf, 0x10cd0, 0x10cd1, 0x10cd2, 0x10cd3, 0x10cd4,
    0x10cd5, 0x10cd6, 0x10cd7, 0x10cd8, 0x10cd9, 0x10cda, 0x10cdb,
    0x10cdc, 0x10cdd, 0x10cde, 0x10cdf, 0x10ce0, 0x10ce1, 0x10ce2,
    0x10ce3, 0x10ce4, 0x10ce5, 0x10ce6, 0x10ce7, 0x10ce8, 0x10ce9,
    0x10cea, 0x10ceb, 0x10cec, 0x10ced, 0x10cee, 0x10cef, 0x10cf0,
    0x10cf1, 0x10cf2, 0x10cfa, 0x10cfb, 0x10cfc, 0x10cfd, 0x10cfe,
    0x10cff, 0x1e800, 0x1e801, 0x1e802, 0x1e803, 0x1e804, 0x1e805,
    0x1e806, 0x1e807, 0x1e808, 0x1e809, 0x1e80a, 0x1e80b, 0x1e80c,
    0x1e80d, 0x1e80e, 0x1e80f, 0x1e810, 0x1e811, 0x1e812, 0x1e813,
    0x1e814, 0x1e815, 0x1e816, 0x1e817, 0x1e818, 0x1e819, 0x1e81a,
    0x1e81b, 0x1e81c, 0x1e81d, 0x1e81e, 0x1e81f, 0x1e820, 0x1e821,
    0x1e822, 0x1e823, 0x1e824, 0x1e825, 0x1e826, 0x1e827, 0x1e828,
    0x1e829, 0x1e82a, 0x1e82b, 0x1e82c, 0x1e82d, 0x1e82e, 0x1e82f,
    0x1e830, 0x1e831, 0x1e832, 0x1e833, 0x1e834, 0x1e835, 0x1e836,
    0x1e837, 0x1e838, 0x1e839, 0x1e83a, 0x1e83b, 0x1e83c, 0x1e83d,
    0x1e83e, 0x1e83f, 0x1e840, 0x1e841, 0x1e842, 0x1e843, 0x1e844,
    0x1e845, 0x1e846, 0x1e847, 0x1e848, 0x1e849, 0x1e84a, 0x1e84b,
    0x1e84c, 0x1e84d, 0x1e84e, 0x1e84f, 0x1e850, 0x1e851, 0x1e852,
    0x1e853, 0x1e854, 0x1e855, 0x1e856, 0x1e857, 0x1e858, 0x1e859,
    0x1e85a, 0x1e85b, 0x1e85c, 0x1e85d, 0x1e85e, 0x1e85f, 0x1e860,
    0x1e861, 0x1e862, 0x1e863, 0x1e864, 0x1e865, 0x1e866, 0x1e867,
    0x1e868, 0x1e869, 0x1e86a, 0x1e86b, 0x1e86c, 0x1e86d, 0x1e86e,
    0x1e86f, 0x1e870, 0x1e871, 0x1e872, 0x1e873, 0x1e874, 0x1e875,
    0x1e876, 0x1e877, 0x1e878, 0x1e879, 0x1e87a, 0x1e87b, 0x1e87c,
    0x1e87d, 0x1e87e, 0x1e87f, 0x1e880, 0x1e881, 0x1e882, 0x1e883,
    0x1e884, 0x1e885, 0x1e886, 0x1e887, 0x1e888, 0x1e889, 0x1e88a,
    0x1e88b, 0x1e88c, 0x1e88d, 0x1e88e, 0x1e88f, 0x1e890, 0x1e891,
    0x1e892, 0x1e893, 0x1e894, 0x1e895, 0x1e896, 0x1e897, 0x1e898,
    0x1e899, 0x1e89a, 0x1e89b, 0x1e89c, 0x1e89d, 0x1e89e, 0x1e89f,
    0x1e8a0, 0x1e8a1, 0x1e8a2, 0x1e8a3, 0x1e8a4, 0x1e8a5, 0x1e8a6,
    0x1e8a7, 0x1e8a8, 0x1e8a9, 0x1e8aa, 0x1e8ab, 0x1e8ac, 0x1e8ad,
    0x1e8ae, 0x1e8af, 0x1e8b0, 0x1e8b1, 0x1e8b2, 0x1e8b3, 0x1e8b4,
    0x1e8b5, 0x1e8b6, 0x1e8b7, 0x1e8b8, 0x1e8b9, 0x1e8ba, 0x1e8bb,
    0x1e8bc, 0x1e8bd, 0x1e8be, 0x1e8bf, 0x1e8c0, 0x1e8c1, 0x1e8c2,
    0x1e8c3, 0x1e8c4, 0x1e8c7, 0x1e8c8, 0x1e8c9, 0x1e8ca, 0x1e8cb,
    0x1e8cc, 0x1e8cd, 0x1e8ce, 0x1e8cf, 0x1e900, 0x1e901, 0x1e902,
    0x1e903, 0x1e904, 0x1e905, 0x1e906, 0x1e907, 0x1e908, 0x1e909,
    0x1e90a, 0x1e90b, 0x1e90c, 0x1e90d, 0x1e90e, 0x1e90f, 0x1e910,
    0x1e911, 0x1e912, 0x1e913, 0x1e914, 0x1e915, 0x1e916, 0x1e917,
    0x1e918, 0x1e919, 0x1e91a, 0x1e91b, 0x1e91c, 0x1e91d, 0x1e91e,
    0x1e91f, 0x1e920, 0x1e921, 0x1e922, 0x1e923, 0x1e924, 0x1e925,
    0x1e926, 0x1e927, 0x1e928, 0x1e929, 0x1e92a, 0x1e92b, 0x1e92c,
    0x1e92d, 0x1e92e, 0x1e92f, 0x1e930, 0x1e931, 0x1e932, 0x1e933,
    0x1e934, 0x1e935, 0x1e936, 0x1e937, 0x1e938, 0x1e939, 0x1e93a,
    0x1e93b, 0x1e93c, 0x1e93d, 0x1e93e, 0x1e93f, 0x1e940, 0x1e941,
    0x1e942, 0x1e943, 0x1e950, 0x1e951, 0x1e952, 0x1e953, 0x1e954,
    0x1e955, 0x1e956, 0x1e957, 0x1e958, 0x1e959, 0x1e95e, 0x1e95f
};
