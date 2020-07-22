#ifndef SPINNER_H
#define SPINNER_H

#ifdef __cplusplus
extern "C" {
#endif
void init_spinner(const char *msg);
void finish_spinner(const char *msg);
void update_spinner(double completed);
#ifdef __cplusplus
}
#endif

#endif /* SPINNER_H */
