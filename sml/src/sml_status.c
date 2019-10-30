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

#include <sml/sml_status.h>

sml_status *sml_status_init() {
	sml_status *status = (sml_status *)malloc(sizeof(sml_status));
	*status = (sml_status){.type = SML_TYPE_UNSIGNED, .data.status8 = NULL};

	return status;
}

sml_status *sml_status_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return NULL;
	}

	int max = 1;
	int type = sml_buf_get_next_type(buf);
	unsigned char byte = sml_buf_get_current_byte(buf);

	sml_status *status = sml_status_init();
	status->type = type;
	switch (type) {
	case SML_TYPE_UNSIGNED:
		// get maximal size, if not all bytes are used (example: only 6 bytes for a u64)
		while (max < ((byte & SML_LENGTH_FIELD) - 1)) {
			max <<= 1;
		}

		status->data.status8 = sml_number_parse(buf, type, max);
		status->type |= max;
		break;
	default:
		buf->error = 1;
		break;
	}
	if (sml_buf_has_errors(buf)) {
		sml_status_free(status);
		return NULL;
	}

	return status;
}

void sml_status_write(sml_status *status, sml_buffer *buf) {
	if (status == 0) {
		sml_buf_optional_write(buf);
		return;
	}
	sml_number_write(status->data.status8, (status->type & SML_TYPE_FIELD),
					 (status->type & SML_LENGTH_FIELD), buf);
}

void sml_status_free(sml_status *status) {
	if (status) {
		sml_number_free(status->data.status8);
		free(status);
	}
}
