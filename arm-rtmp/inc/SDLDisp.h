/*
 * SDLDisp.h
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#ifndef SDLDISP_H_
#define SDLDISP_H_

#include "Common.h"

class SDLDisp {
	//Refresh Event
	#define REFRESH_EVENT  (SDL_USEREVENT + 1)
	//Break
	#define BREAK_EVENT  (SDL_USEREVENT + 2)

public:
	SDLDisp(int screen_w,int screen_h);
	virtual ~SDLDisp();

	int initSDL(char *winName);
	int closeSDL();
	int refresh_video(void *opaque);
	int sdlDisp();
	int doSDLDisp(uint8_t *buf);
	void DisplayYUV420P(uint8_t *buf, uint32_t w, uint32_t h, SDL_Overlay *overlay);
	void DisplayYUV420(uint8_t *buf, uint32_t w, uint32_t h, SDL_Overlay *overlay);
	void DisplayYUV422(uint8_t *buf, uint32_t w, uint32_t h, SDL_Overlay *overlay);
	void normalSDLDisp(uint8_t *buf);

//	static void *sdlcontol(void *void_this)
//	{
//		//return static_cast<SDLDisp*>(void_this)->refresh_video;
//		static_cast<SDLDisp*>(void_this)->refresh_video();
//	}

private:
	int screen_w;
	int screen_h;
	long framesize;
	uint8_t *buffer;

	SDL_Surface *screen;
	SDL_Overlay *overlay;
	SDL_Rect rect;

	//SDL_Thread *refresh_thread;
	SDL_Event event;
	int sdl_running;
	int thread_exit;

	pthread_t dispTid;
	pthread_t controlTid;
	pthread_rwlock_t rwlock;
};

#endif /* SDLDISP_H_ */
