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

#include <sml/sml_number.h>
#include <sml/sml_shared.h>
#include <sml/sml_time.h>
#include <stdio.h>

sml_timestamp_local *sml_timestamp_local_init() {
	sml_timestamp_local *t = (sml_timestamp_local *)malloc(sizeof(sml_timestamp_local));
	*t = (sml_timestamp_local){.timestamp = NULL, .local_offset = NULL, .season_time_offset = NULL};
	return t;
}

sml_timestamp_local *sml_timestamp_local_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return NULL;
	}

	if (sml_buf_get_next_length(buf) != 3) {
		buf->error = 1;
		goto error;
	}

	sml_timestamp_local *time = sml_timestamp_local_init();

	time->timestamp = sml_u32_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	time->local_offset = sml_i16_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	time->season_time_offset = sml_i16_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	return time;

error:
	sml_timestamp_local_free(time);
	return NULL;
}

void sml_timestamp_local_write(sml_timestamp_local *time, sml_buffer *buf) {
	if (time == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_u32_write(time->timestamp, buf);
	sml_i16_write(time->local_offset, buf);
	sml_i16_write(time->season_time_offset, buf);
}

void sml_timestamp_local_free(sml_timestamp_local *time) {
	if (time) {
		sml_number_free(time->timestamp);
		sml_number_free(time->local_offset);
		sml_number_free(time->season_time_offset);
		free(time);
	}
}

sml_time *sml_time_init() {
	sml_time *t = (sml_time *)malloc(sizeof(sml_time));
	*t = (sml_time){.tag = NULL, .data.sec_index = NULL};
	return t;
}

sml_time *sml_time_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return NULL;
	}

	sml_time *tme = sml_time_init();

	// workaround Holley DTZ541
	// if SML_ListEntry valTime (SML_Time) is given there are missing bytes:
	// 0x72: indicate a list for SML_Time with 2 entries
	// 0x62 0x01: indicate secIndex
	// instead, the DTZ541 starts with 0x65 + 4 bytes secIndex
	// the workaround will add this information during parsing
	if (sml_buf_get_current_byte(buf) == (SML_TYPE_UNSIGNED | 5)) {
		tme->tag = malloc(sizeof(u8));
		*(tme->tag) = SML_TIME_SEC_INDEX;
	} else {
		if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
			buf->error = 1;
			goto error;
		}

		if (sml_buf_get_next_length(buf) != 2) {
			buf->error = 1;
			goto error;
		}

		tme->tag = sml_u8_parse(buf);
		if (sml_buf_has_errors(buf))
			goto error;
	}

	int type = sml_buf_get_next_type(buf);
	switch (type) {
	case SML_TYPE_UNSIGNED:
		tme->data.timestamp = sml_u32_parse(buf);
		if (sml_buf_has_errors(buf))
			goto error;
		break;
	case SML_TYPE_LIST:
		tme->data.local_timestamp = sml_timestamp_local_parse(buf);
		if (sml_buf_has_errors(buf))
			goto error;
		break;
	default:
		goto error;
	}

	return tme;

error:
	sml_time_free(tme);
	return NULL;
}

void sml_time_write(sml_time *t, sml_buffer *buf) {
	if (t == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 2);
	sml_u8_write(t->tag, buf);
	if (*t->tag == SML_TIME_LOCAL_TIMESTAMP) {
		sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 3);
		sml_timestamp_local_write(t->data.local_timestamp, buf);
	} else {
		sml_u32_write(t->data.timestamp, buf);
	}
}

void sml_time_free(sml_time *tme) {
	if (tme) {
		if (tme->tag) {
			switch (*tme->tag) {
			case SML_TIME_LOCAL_TIMESTAMP:
				sml_timestamp_local_free(tme->data.local_timestamp);
				break;
			default:
				sml_number_free(tme->data.timestamp);
			}
		}
		sml_number_free(tme->tag);
		free(tme);
	}
}
