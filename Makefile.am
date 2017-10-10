noinst_HEADERS = Ball.h ControlListener.h GameObject.h OgreMotionState.h Overlay.h Player.h PlayerCamera.h PlayingField.h Simulator.h Wall.h BaseApplication.h ThreeDPool.h Stick.h

bin_PROGRAMS = ThreeDPool
ThreeDPool_CPPFLAGS = -I$(top_srcdir)
ThreeDPool_SOURCES = Ball.cpp ControlListener.cpp GameObject.cpp Overlay.cpp Player.cpp PlayerCamera.cpp PlayingField.cpp Simulator.cpp Wall.cpp BaseApplication.cpp ThreeDPool.cpp Stick.cpp
ThreeDPool_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
ThreeDPool_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
ThreeDPool_LDFLAGS = -lOgreOverlay -lboost_system -R/lusr/lib/cegui-0.8

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
