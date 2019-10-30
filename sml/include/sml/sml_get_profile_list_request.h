// Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed
// DAI-Labor, TU-Berlin
//
// This file is part of libSML.
//
// libSML is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libSML is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libSML.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SML_GET_PROFILE_LIST_REQUEST_H_
#define SML_GET_PROFILE_LIST_REQUEST_H_

#include "sml_get_profile_pack_request.h"

#ifdef __cplusplus
extern "C" {
#endif

// Apparently SML_GetProfilePack.Req is the same as SML_GetProfileList.Req
typedef sml_get_profile_pack_request sml_get_profile_list_request;

#define sml_get_profile_list_request_init() sml_get_profile_pack_request_init()
#define sml_get_profile_list_request_parse(buf) sml_get_profile_pack_request_parse(buf)
#define sml_get_profile_list_request_write(msg, buf) sml_get_profile_pack_request_write(msg, buf)
#define sml_get_profile_list_request_free(msg) sml_get_profile_pack_request_free(msg)

#ifdef __cplusplus
}
#endif

#endif /* SML_GET_PROFILE_LIST_REQUEST_H_ */
