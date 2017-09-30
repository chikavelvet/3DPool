noinst_HEADERS = Ball.h ControlListener.h GameObject.h OgreMotionState.h Overlay.h Player.h PlayerCamera.h PlayingField.h Simulator.h Wall.h BaseApplication.h ThreeDPool.h

bin_PROGRAMS = 3dpool
3dpool_CPPFLAGS = -I$(top_srcdir)
3dpool_SOURCES = Ball.cpp ControlListener.cpp GameObject.cpp Overlay.cpp Player.cpp PlayerCamera.cpp PlayingField.cpp Simulator.cpp Wall.cpp BaseApplication.cpp ThreeDPool.cpp
3dpool_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
3dpool_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
3dpool_LDFLAGS = -lOgreOverlay -lboost_system

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
