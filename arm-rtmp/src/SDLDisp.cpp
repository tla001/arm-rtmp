/*
 * SDLDisp.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: tla001
 */

#include "SDLDisp.h"
/*
 * display different data
 * 1.framesize 				420p:screen_w*screen_h*3/2	422:screen_w*screen_h*2
 * 2.SDL_CreateYUVOverlay 	420p:SDL_IYUV_OVERLAY		422:SDL_YUY2_OVERLAY
 * 3.dispfuction			420p:DisplayYUV420P			422:DisplayYUV422
 */

SDLDisp::SDLDisp(int screen_w,int screen_h) {
	// TODO Auto-generated constructor stub
	this->screen_w=screen_w;
	this->screen_h=screen_h;
	this->framesize=screen_w*screen_h*3/2;
	this->buffer=NULL;
	this->sdl_running=1;
	this->thread_exit=0;
}

SDLDisp::~SDLDisp() {
	// TODO Auto-generated destructor stub
	this->sdl_running=0;
	closeSDL();
}
int SDLDisp::initSDL(char *winName)
{
	buffer=(uint8_t*)malloc(sizeof(uint8_t)*framesize);
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(screen_w, screen_h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if(!screen) {
		printf("SDL: could not set video mode - exiting:%s\n",SDL_GetError());
		return -1;
	}
	/* Set the window name */
	 SDL_WM_SetCaption(winName, NULL);
	 //yuv420P	run with DisplayYUV420P
	// overlay = SDL_CreateYUVOverlay(screen_w, screen_h,SDL_IYUV_OVERLAY, screen);
	 //yuv422 run with DisplayYUV422
	 overlay = SDL_CreateYUVOverlay(screen_w, screen_h,SDL_IYUV_OVERLAY, screen);

	rect.x=0;
	rect.y = 0;
	rect.w = screen_w;
	rect.h = screen_h;
	//refresh_thread = SDL_CreateThread(sdlcontol,NULL);
	//pthread_create(&controlTid,NULL,sdlcontol, static_cast<void*>(this));
	//³õÊ¼»¯¶ÁÐ´Ëù
//	int ret=pthread_rwlock_init(&rwlock, NULL);
//	if(ret<0){
//		printf("init wrlock failed\n");
//		return -1;
//	}
	//pthread_create(&dispTid,NULL,&sdlDisp,NULL);

	return 0;
}
int SDLDisp::closeSDL()
{
	if(buffer!=NULL)
		free(buffer);
	thread_exit=1;
	SDL_FreeYUVOverlay(overlay);
	SDL_FreeSurface(screen);
	SDL_Quit();
	printf("sdl end\n");
	return 0;
}
int SDLDisp::sdlDisp()
{
	printf("sdl disp start\n");
	while(sdl_running){
		SDL_WaitEvent(&event);
		if(event.type==REFRESH_EVENT){
			SDL_LockSurface(screen);
			pthread_rwlock_rdlock(&rwlock);
			DisplayYUV420P(buffer, screen_w, screen_h, overlay);
			pthread_rwlock_unlock(&rwlock);
			SDL_UnlockSurface(screen);
			SDL_DisplayYUVOverlay(overlay, &rect);
			//Update Screen
			SDL_Flip(screen);
		}else if(event.type==SDL_QUIT){
			thread_exit=1;
			sdl_running=0;
		}else if(event.type==BREAK_EVENT){
			break;
		}
	}
	thread_exit=1;
	printf("sdl disp end\n");
	return 0;
}
int SDLDisp::refresh_video(void *opaque){
	thread_exit=0;
	while (thread_exit==0) {
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	thread_exit=0;
	//Break
	SDL_Event event;
	event.type = BREAK_EVENT;
	SDL_PushEvent(&event);
	return 0;
}
int SDLDisp::doSDLDisp(uint8_t *buf)
{
	pthread_rwlock_wrlock(&rwlock);
	memcpy(buffer,buf,framesize);
	pthread_rwlock_unlock(&rwlock);
	return 0;
}
void SDLDisp::normalSDLDisp(uint8_t *buf)
{
	memcpy(buffer,buf,framesize);
	SDL_LockSurface(screen);
	DisplayYUV420P(buffer, screen_w, screen_h, overlay);//420p
	//DisplayYUV422(buffer, screen_w, screen_h, overlay);//422
	SDL_UnlockSurface(screen);
	SDL_DisplayYUVOverlay(overlay, &rect);
	//Update Screen
	SDL_Flip(screen);
}
void SDLDisp::DisplayYUV420P(uint8_t *buf, uint32_t w, uint32_t h, SDL_Overlay *overlay)
{
	/* Fill in video data */
	//uint32_t yuv_size = (w*h) * 3/2;
	uint8_t * y_video_data = (uint8_t*)buf;
	uint8_t * u_video_data = (uint8_t*)(buf + w * h);
	uint8_t * v_video_data = (uint8_t*)(u_video_data + (w * h / 4));

	/* Fill in pixel data - the pitches array contains the length of a line in each plane */
	SDL_LockYUVOverlay(overlay);
	memcpy(overlay->pixels[0], y_video_data, w * h);
	memcpy(overlay->pixels[1], u_video_data, w * h / 4);
	memcpy(overlay->pixels[2], v_video_data, w * h / 4);

	SDL_UnlockYUVOverlay(overlay);
}
void SDLDisp::DisplayYUV420(uint8_t *buf, uint32_t w, uint32_t h, SDL_Overlay *overlay)
{
	/* Fill in video data */
	//uint32_t yuv_size = (w*h) * 3/2;
	uint8_t * y_video_data = (uint8_t*)buf;
	uint8_t * v_video_data = (uint8_t*)(buf + w * h);
	uint8_t * u_video_data = (uint8_t*)(v_video_data + (w * h / 4));

	/* Fill in pixel data - the pitches array contains the length of a line in each plane */
	SDL_LockYUVOverlay(overlay);
	memcpy(overlay->pixels[0], y_video_data, w * h);
	memcpy(overlay->pixels[1], u_video_data, w * h / 4);
	memcpy(overlay->pixels[2], v_video_data, w * h / 4);

	SDL_UnlockYUVOverlay(overlay);
}
void SDLDisp::DisplayYUV422(uint8_t *buf, uint32_t w, uint32_t h, SDL_Overlay *overlay)
{
	/* Fill in pixel data - the pitches array contains the length of a line in each plane */
	SDL_LockYUVOverlay(overlay);
	memcpy(overlay->pixels[0], buf, w * h*2);
	SDL_UnlockYUVOverlay(overlay);
}
