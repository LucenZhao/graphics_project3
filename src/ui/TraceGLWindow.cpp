// TraceGLWindow
// A subclass of FL_GL_Window that handles drawing the traced image to the screen
// 

#include "TraceGLWindow.h"
#include "../RayTracer.h"
#include <FL/fl_ask.h>
#include "../fileio/bitmap.h"

TraceGLWindow::TraceGLWindow(int x, int y, int w, int h, const char *l)
			: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth = w;
	m_nWindowHeight = h;
}

int TraceGLWindow::handle(int event)
{
	// disable all mouse and keyboard events
	return 1;
}

void TraceGLWindow::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		m_nWindowWidth=w();
		m_nWindowHeight=h();
	}

	glClear( GL_COLOR_BUFFER_BIT );

	unsigned char* buf;
	raytracer->getBuffer(buf, m_nDrawWidth, m_nDrawHeight);

	if ( buf ) {
		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, 0 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_nDrawWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( m_nDrawWidth, m_nDrawHeight, GL_RGB, GL_UNSIGNED_BYTE, buf );
	}
		
	glFlush();
}

void TraceGLWindow::refresh()
{
	redraw();
}

void TraceGLWindow::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
	m_nWindowWidth=w();
	m_nWindowHeight=h();
}

void TraceGLWindow::saveImage(char *iname)
{
	unsigned char* buf;

	raytracer->getBuffer(buf, m_nDrawWidth, m_nDrawHeight);
	if (buf)
		writeBMP(iname, m_nDrawWidth, m_nDrawHeight, buf); 
}

void TraceGLWindow::loadBackground(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return;
	}

	if (width != m_nDrawWidth || height != m_nDrawHeight)
	{
		fl_alert("Size of bitmap file different from the scene.");
		return;
	}


	// release old storage
	//if (m_ucBitmapBG) delete[] m_ucBitmapBG;
	//m_ucBitmapBG = new unsigned char[width*height * 3];
	m_ucBitmapBG = data;
	havingBG = true;
}

void TraceGLWindow::loadTexture(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return;
	}


	// release old storage
	//if (m_ucBitmapBG) delete[] m_ucBitmapBG;
	//m_ucBitmapBG = new unsigned char[width*height * 3];
	m_ucBitmapTexture = data;
	havingTexture = true;
}

void TraceGLWindow::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
}