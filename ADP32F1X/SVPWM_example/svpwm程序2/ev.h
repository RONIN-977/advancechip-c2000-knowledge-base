#ifndef __EV_H__
#define __EV_H__

#define TCBY2 2000

typedef struct {
                 void(*init)(void*);
                 void(*pwm)(void*);
                 void(*open)(void*);
                 void(*close)(void*);

                 unsigned  vect;
                 unsigned  t1;
                 unsigned  tm;
} EV;

#define EV_DEFAULTS {        \
  (void(*)(void*)) ev_init,\
  (void(*)(void*)) ev_pwm,\
  (void(*)(void*)) ev_open,\
  (void(*)(void*)) ev_close,\
}

typedef EV *EV_Handle;

void ev_init (EV_Handle);
void ev_pwm  (EV_Handle);
void ev_open (EV_Handle);
void ev_close (EV_Handle);

#endif