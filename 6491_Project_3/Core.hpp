#ifndef CORE_HPP
#define CORE_HPP

#include <time.h>

extern float refreshRate;

void initCore(void);
void uninitCore(void);

void run(void);
void update(void);
//void render(void);

//thread to call redraw;

#endif
