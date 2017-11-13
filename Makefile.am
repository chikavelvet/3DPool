noinst_HEADERS = Ball.h ControlListener.h Enums.h GameObject.h OgreMotionState.h Overlay.h Player.h PlayerCamera.h PlayingField.h Simulator.h Room.h Wall.h BaseApplication.h ThreeDPool.h Stick.h Pocket.h NetManager.h Component.h PhysicsComponent.h GraphicsComponent.h ComponentNotFoundException.h


CEGUI_CFLAGS = -pthread -I/lusr/opt/cegui-0.8.7/include/cegui-0 -I/usr/include/OGRE
CEGUI_LIBS = -L/lusr/opt/cegui-0.8.7/lib -lCEGUIOgreRenderer-0 -lOgreMain -lpthread -lCEGUIBase-0


bin_PROGRAMS = ThreeDPool
ThreeDPool_CPPFLAGS = -I$(top_srcdir)
ThreeDPool_SOURCES = Ball.cpp ControlListener.cpp GameObject.cpp Overlay.cpp Player.cpp PlayerCamera.cpp PlayingField.cpp Simulator.cpp Room.cpp Wall.cpp BaseApplication.cpp ThreeDPool.cpp Stick.cpp Pocket.cpp NetManager.cpp Component.cpp PhysicsComponent.cpp GraphicsComponent.cpp ComponentNotFoundException.cpp
ThreeDPool_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(SDL_CFLAGS) -lSDL -lSDL_mixer -lSDL_net
ThreeDPool_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(SDL_LIBS)
ThreeDPool_LDFLAGS = -lOgreOverlay -lboost_system -R/lusr/lib/cegui-0.8

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
