#ifndef TREE_STRING_VIEW_H
#define TREE_STRING_VIEW_H


#include <stddef.h>
#include <string.h>
#define SV_Fmt "%.*s"
#define SV_Arg(s) (s).count, (s).data

typedef struct {
    const char *data;
    size_t count;
} String_View;

String_View sv(const char * cstr);
void sv_chop_right(String_View* sv, size_t n);
void sv_chop_left(String_View* sv, size_t n);
void sv_trim (String_View *sv);
String_View sv_before_token (String_View *sv, char delim);
String_View sv_after_token (String_View sv, char delim);
int sv_equals(String_View a, String_View b);
int sv_cmp(String_View a, String_View b);
int sv_has_prefix(String_View sv, String_View prefix);
int sv_contains(String_View base, String_View buscado);
String_View sv_copy(String_View sv);

#endif
