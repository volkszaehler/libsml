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

#include <sml/sml_list.h>
#include <sml/sml_octet_string.h>
#include <sml/sml_shared.h>
#include <sml/sml_status.h>
#include <sml/sml_time.h>
#include <sml/sml_value.h>
#include <stdio.h>

// sml_sequence;

sml_sequence *sml_sequence_init(void (*elem_free)(void *elem)) {
	sml_sequence *seq = (sml_sequence *)malloc(sizeof(sml_sequence));
	*seq = (sml_sequence){.elems = NULL, .elems_len = 0, .elem_free = elem_free};

	return seq;
}

sml_sequence *sml_sequence_parse(sml_buffer *buf, void *(*elem_parse)(sml_buffer *buf),
								 void (*elem_free)(void *elem)) {
	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		goto error;
	}

	sml_sequence *seq = sml_sequence_init(elem_free);
	int i, len = sml_buf_get_next_length(buf);
	void *p;
	for (i = 0; i < len; i++) {
		p = elem_parse(buf);
		if (sml_buf_has_errors(buf)) {
			goto haserrors;
		}
		sml_sequence_add(seq, p);
	}

	return seq;

haserrors:
	sml_sequence_free(seq);
error:
	buf->error = 1;
	return NULL;
}

void sml_sequence_write(sml_sequence *seq, sml_buffer *buf,
						void (*elem_write)(void *elem, sml_buffer *buf)) {
	if (seq == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, seq->elems_len);

	int i;
	for (i = 0; i < seq->elems_len; i++) {
		elem_write((seq->elems)[i], buf);
	}
}

void sml_sequence_free(sml_sequence *seq) {
	if (seq) {
		int i;
		for (i = 0; i < seq->elems_len; i++) {
			seq->elem_free((seq->elems)[i]);
		}

		if (seq->elems != 0) {
			free(seq->elems);
		}

		free(seq);
	}
}

void sml_sequence_add(sml_sequence *seq, void *new_entry) {
	seq->elems_len++;
	seq->elems = (void **)realloc(seq->elems, sizeof(void *) * seq->elems_len);
	seq->elems[seq->elems_len - 1] = new_entry;
}

// sml_list;

sml_list *sml_list_init() {
	sml_list *s = (sml_list *)malloc(sizeof(sml_list));
	*s = (sml_list){.obj_name = NULL,
					.status = NULL,
					.val_time = NULL,
					.unit = NULL,
					.scaler = NULL,
					.value = NULL,
					.value_signature = NULL,
					.next = NULL};
	return s;
}

void sml_list_add(sml_list *list, sml_list *new_entry) { list->next = new_entry; }

sml_list *sml_list_entry_parse(sml_buffer *buf) {
	static const unsigned char dzg_power_name[] = {1, 0, 16, 7, 0, 255};
	u8 value_tl, value_len_more;
	sml_list *l = NULL;

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 7) {
		buf->error = 1;
		goto error;
	}
	l = sml_list_init();

	l->obj_name = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	l->status = sml_status_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	l->val_time = sml_time_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	l->unit = sml_u8_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	l->scaler = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	if (buf->cursor >= buf->buffer_len) {
		goto error;
	}

	value_tl = sml_buf_get_current_byte(buf);
	value_len_more = value_tl & (SML_ANOTHER_TL | SML_LENGTH_FIELD);
	l->value = sml_value_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	l->value_signature = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf))
		goto error;

	if ((buf->workarounds & SML_WORKAROUND_DZG_NEGATIVE) && l->obj_name &&
		l->obj_name->len == sizeof(dzg_power_name) &&
		memcmp(l->obj_name->str, dzg_power_name, sizeof(dzg_power_name)) == 0 && l->value &&
		(value_len_more == 1 || value_len_more == 2 || value_len_more == 3)) {
		l->value->type &= ~SML_TYPE_FIELD;
		l->value->type |= SML_TYPE_UNSIGNED;
	}

	return l;

error:
	buf->error = 1;
	if (l) {
		sml_list_free(l);
	}
	return NULL;
}

sml_list *sml_list_parse(sml_buffer *buf) {
	sml_list *ret = NULL, **pos = &ret;
	int elems;

	if (sml_buf_optional_is_skipped(buf)) {
		return NULL;
	}

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		return NULL;
	}

	elems = sml_buf_get_next_length(buf);

	while (elems > 0) {
		*pos = sml_list_entry_parse(buf);
		if (sml_buf_has_errors(buf))
			goto error;
		pos = &(*pos)->next;
		elems--;
	}

	return ret;

error:
	buf->error = 1;
	sml_list_free(ret);
	return NULL;
}

void sml_list_entry_write(sml_list *list, sml_buffer *buf) {
	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 7);
	sml_octet_string_write(list->obj_name, buf);
	sml_status_write(list->status, buf);
	sml_time_write(list->val_time, buf);
	sml_u8_write(list->unit, buf);
	sml_i8_write(list->scaler, buf);
	sml_value_write(list->value, buf);
	sml_octet_string_write(list->value_signature, buf);
}

void sml_list_write(sml_list *list, sml_buffer *buf) {
	if (list == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_list *i = list;
	int len = 0;
	while (i) {
		i = i->next;
		len++;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, len);

	i = list;
	while (i) {
		sml_list_entry_write(i, buf);
		i = i->next;
	}
}

void sml_list_entry_free(sml_list *list) {
	if (list) {
		sml_octet_string_free(list->obj_name);
		sml_status_free(list->status);
		sml_time_free(list->val_time);
		sml_number_free(list->unit);
		sml_number_free(list->scaler);
		sml_value_free(list->value);
		sml_octet_string_free(list->value_signature);

		free(list);
	}
}

void sml_list_free(sml_list *list) {
	if (list) {
		sml_list *f = list;
		sml_list *n = list->next;

		while (f) {
			sml_list_entry_free(f);
			f = n;
			if (f) {
				n = f->next;
			}
		}
	}
}
