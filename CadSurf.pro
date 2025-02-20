######################################################################
# Automatically generated by qmake (1.07a) Tue Jul 18 22:10:04 2006
######################################################################

TEMPLATE = app
INCLUDEPATH += . /
CONFIG	+= qt warn_on debug opengl
DEFINES	+= 'VERSION="5.0"' QT_CLEAN_NAMESPACE
INCLUDEPATH += . /home/sharjith/include
LIBS +=		 -L/home/sharjith/lib64 \
		 -lVKGeom -lVKGraphic -lVKCollection -lglut
TARGET = cadsurf

unix:{
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
win32:{
    UI_DIR = ui
    MOC_DIR = moc
    OBJECTS_DIR = obj
}

# Input
HEADERS += CadSurf.h \
           CadSurfDoc.h \
           CadSurfView.h \        
           MaterialDlg.ui.h \
           MDIWindow.h \
	   GLDisplayContext.cpp \
	   UserSurface.h \
           QGLView.h        
INTERFACES += MaterialDlg.ui GridSettings.ui
SOURCES += CadSurf.cpp \
           CadSurfDoc.cpp \
           CadSurfView.cpp \           
           main.cpp \
           MDIWindow.cpp \
	   GLDisplayContext.cpp \
	   UserSurface.cpp \
           QGLView.cpp           
