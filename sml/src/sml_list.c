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
#include <sml/sml_shared.h>
#include <sml/sml_time.h>
#include <sml/sml_octet_string.h>
#include <sml/sml_status.h>
#include <sml/sml_value.h>
#include <stdio.h>

// sml_sequence;

sml_sequence *sml_sequence_init(void (*elem_free) (void *elem)) {
	sml_sequence *seq = (sml_sequence *) malloc(sizeof(sml_sequence));
	*seq = ( sml_sequence ) {
		.elems = NULL,
		.elems_len = 0,
		.elem_free = elem_free
	};

	return seq;
}

sml_sequence *sml_sequence_parse(sml_buffer *buf, void *(*elem_parse) (sml_buffer *buf), void (*elem_free) (void *elem)) {
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

void sml_sequence_write(sml_sequence *seq, sml_buffer *buf, void (*elem_write) (void *elem, sml_buffer *buf)) {
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
	seq->elems = (void **) realloc(seq->elems, sizeof(void *) * seq->elems_len);
	seq->elems[seq->elems_len - 1] = new_entry;
}


// sml_list;

sml_list *sml_list_init(){
	sml_list *s = (sml_list *)malloc(sizeof(sml_list));
	*s = ( sml_list ) {
		.obj_name = NULL,
		.status = NULL,
		.val_time = NULL,
		.unit = NULL,
		.scaler = NULL,
		.value = NULL,
		.value_signature = NULL,
		.next = NULL
	};
	return s;
}

void sml_list_add(sml_list *list, sml_list *new_entry) {
	list->next = new_entry;
}

sml_list *sml_list_entry_parse(sml_buffer *buf) {
	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 7) {
		buf->error = 1;
		goto error;
	}
	sml_list *l = sml_list_init();

	l->obj_name = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	l->status = sml_status_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	l->val_time = sml_time_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	l->unit = sml_u8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	l->scaler = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	l->value = sml_value_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	l->value_signature = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	return l;

// This function doesn't free the allocated memory in error cases,
// this is done in sml_list_parse.
error:
	buf->error = 1;
	return NULL;
}

sml_list *sml_list_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return NULL;
	}

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		return NULL;
	}

	sml_list *first = 0;
	sml_list *last = 0;
	int elems;

	elems = sml_buf_get_next_length(buf);

	if (elems > 0) {
		first = sml_list_entry_parse(buf);
		if (sml_buf_has_errors(buf)) goto error;
		last = first;
		elems--;
	}

	while(elems > 0) {
		last->next = sml_list_entry_parse(buf);
		if (sml_buf_has_errors(buf)) goto error;
		last = last->next;
		elems--;
	}

	return first;

error:
	buf->error = 1;
	sml_list_free(first);
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

void sml_list_write(sml_list *list, sml_buffer *buf){
	if (list == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_list *i = list;
	int len = 0;
	while(i) {
		i = i->next;
		len++;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, len);

	i = list;
	while(i) {
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

		while(f) {
			sml_list_entry_free(f);
			f = n;
			if (f) {
				n = f->next;
			}
		}
	}
}

