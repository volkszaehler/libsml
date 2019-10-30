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

#ifndef SML_CLOSE_RESPONSE_H_
#define SML_CLOSE_RESPONSE_H_

#include "sml_close_request.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef sml_close_request sml_close_response;

sml_close_response *sml_close_response_init();
sml_close_response *sml_close_response_parse(sml_buffer *buf);
void sml_close_response_write(sml_close_response *msg, sml_buffer *buf);
void sml_close_response_free(sml_close_response *msg);

#ifdef __cplusplus
}
#endif

#endif /* SML_CLOSE_RESPONSE_H_ */
