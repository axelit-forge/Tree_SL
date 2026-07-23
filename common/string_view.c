#include "string_view.h"

String_View sv(const char * cstr) {
    return (String_View){
        .data = cstr,
        .count = strlen(cstr)
    };
}

void sv_chop_left(String_View* sv, size_t n) {
    if (n > sv->count) n = sv->count;
    sv->count -= n;
    sv->data  += n;
}

void sv_chop_right(String_View* sv, size_t n) {
    if (n > sv->count) n = sv->count;
    sv->count -= n;
}

void sv_trim_left (String_View *sv) {
    while (sv->count > 0 && sv->data[0] == ' ') {
        sv_chop_left(sv, 1);
    }
}

void sv_trim_right (String_View *sv) {
    while (sv->count > 0 && sv->data[sv->count-1] == ' ') {
        sv_chop_right(sv, 1);
    }
}

void sv_trim (String_View *sv) {
    sv_trim_left(sv);
    sv_trim_right(sv);
}

String_View sv_before_token (String_View *sv, char delim) {
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i++;
    }
    if (i < sv->count) {
        String_View result = {
            .data = sv->data,
            .count = i
        };
        sv_chop_left(sv, i+1);
        return result;
    }
    String_View result = *sv;
    sv_chop_left(sv, sv->count);
    return result;
}
static int bytes_equals(const char *a, const char *b, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int sv_equals(String_View a, String_View b) {
    if (a.count != b.count) {
        return 0;
    }
    return bytes_equals(a.data, b.data, a.count);
}

int sv_has_prefix(String_View sv, String_View prefix) {
    if (prefix.count > sv.count) {
        return 0;
    }
    return bytes_equals(sv.data, prefix.data, prefix.count);
}

int sv_contains(String_View base, String_View buscado) {
    if (buscado.count > base.count) {
        return 0;
    }

    if (buscado.count == 0) {
        return 1;
    }

    size_t max_pos = base.count - buscado.count;
    for (size_t i = 0; i <= max_pos; i++) {
        if (bytes_equals(base.data + i, buscado.data, buscado.count)) {
            return 1;
        }
    }

    return 0;
}
String_View sv_after_token(String_View sv, char delim) {
    size_t i = 0;
    while (i < sv.count && sv.data[i] != delim) {
        i++;
    }

    if (i < sv.count) {
        return (String_View){
            .data = sv.data + i + 1,
            .count = sv.count - (i + 1)
        };
    }

    return (String_View){
        .data = sv.data + sv.count,
        .count = 0 };
}

int sv_cmp(String_View a, String_View b) {
    size_t min_len = a.count < b.count ? a.count : b.count;

    for (size_t i = 0; i < min_len; i++) {
        if (a.data[i] != b.data[i]) {
            return (unsigned char)a.data[i] - (unsigned char)b.data[i];
        }
    }

    if (a.count < b.count) return -1;
    if (a.count > b.count) return 1;
    return 0;
}

String_View sv_copy(String_View sv){
    return sv;
}