//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"

static bool done;

//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}

	pUI->m_traceGlWindow->havingBG = false;
	pUI->m_traceGlWindow->havingTexture = false;
}

void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_load_background(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);
	char* background = fl_file_chooser("Open Image?", "*.bmp", NULL);
	if (background != NULL) {
		pUI->m_traceGlWindow->loadBackground(background);
	}
}

void TraceUI::cb_load_texture(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);
	char* texture = fl_file_chooser("Open Image?", "*.bmp", NULL);
	if (texture != NULL) {
		pUI->m_traceGlWindow->loadTexture(texture);
	}
}

void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}

void TraceUI::cb_supersamplingSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nSupersampling = int(((Fl_Slider *)o)->value());
}

void TraceUI::cb_jitterSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nJitter = int(((Fl_Slider *)o)->value());
}

void TraceUI::cb_adaptiveSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAdaptiveDepth = int(((Fl_Slider *)o)->value());
}

void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );

		pUI->m_traceGlWindow->show();

		pUI->raytracer->traceSetup(width, height);
		
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}

				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::cb_softShadowButton(Fl_Widget *o, void*)
{
	((TraceUI*)(o->user_data()))->m_is_enable_soft_shadow ^= true;
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
	// double binding the UI with ray tracer
	tracer->setTraceUI(this);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}

int TraceUI::getSuperSampling()
{
	return m_nSupersampling;
}

int	TraceUI::getJitter()
{
	return m_nJitter;
}
int	TraceUI::getAdaptiveDepth()
{
	return m_nAdaptiveDepth;
}

bool TraceUI::getSoftShadow()
{
	return m_is_enable_soft_shadow;
}

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Load Background...",	FL_ALT + 'b', (Fl_Callback *)TraceUI::cb_load_background },
		{ "&Load Texture...",	FL_ALT + 't', (Fl_Callback *)TraceUI::cb_load_texture },
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	m_nSize = 150;
	m_nSupersampling = 0;
	m_nJitter = 0;
	m_nAdaptiveDepth = 0;

	m_mainWindow = new Fl_Window(100, 100, 350, 400, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 350, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(512);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);

		m_supersamplingSlider = new Fl_Value_Slider(10, 80, 180, 20, "Supersampling");
		m_supersamplingSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_supersamplingSlider->type(FL_HOR_NICE_SLIDER);
		m_supersamplingSlider->labelfont(FL_COURIER);
		m_supersamplingSlider->labelsize(12);
		m_supersamplingSlider->minimum(0);
		m_supersamplingSlider->maximum(5);
		m_supersamplingSlider->step(1);
		m_supersamplingSlider->value(m_nSupersampling);
		m_supersamplingSlider->align(FL_ALIGN_RIGHT);
		m_supersamplingSlider->callback(cb_supersamplingSlides);

		m_jitterSlider = new Fl_Value_Slider(10, 105, 180, 20, "Jitter");
		m_jitterSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_jitterSlider->type(FL_HOR_NICE_SLIDER);
		m_jitterSlider->labelfont(FL_COURIER);
		m_jitterSlider->labelsize(12);
		m_jitterSlider->minimum(0);
		m_jitterSlider->maximum(5);
		m_jitterSlider->step(1);
		m_jitterSlider->value(m_nJitter);
		m_jitterSlider->align(FL_ALIGN_RIGHT);
		m_jitterSlider->callback(cb_jitterSlides);

		m_adaptiveSlider = new Fl_Value_Slider(10, 130, 180, 20, "Adaptive AA depth");
		m_adaptiveSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_adaptiveSlider->type(FL_HOR_NICE_SLIDER);
		m_adaptiveSlider->labelfont(FL_COURIER);
		m_adaptiveSlider->labelsize(12);
		m_adaptiveSlider->minimum(0);
		m_adaptiveSlider->maximum(5);
		m_adaptiveSlider->step(1);
		m_adaptiveSlider->value(m_nAdaptiveDepth);
		m_adaptiveSlider->align(FL_ALIGN_RIGHT);
		m_adaptiveSlider->callback(cb_adaptiveSlides);

		m_distReflSlider = new Fl_Value_Slider(10, 155, 180, 20, "Dist Reflection");
		m_distReflSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_distReflSlider->type(FL_HOR_NICE_SLIDER);
		m_distReflSlider->labelfont(FL_COURIER);
		m_distReflSlider->labelsize(12);
		m_distReflSlider->minimum(0);
		m_distReflSlider->maximum(1);
		m_distReflSlider->step(1);
		m_distReflSlider->value(0);
		m_distReflSlider->align(FL_ALIGN_RIGHT);

		m_distRefrSlider = new Fl_Value_Slider(10, 180, 180, 20, "Dist Refraction");
		m_distRefrSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_distRefrSlider->type(FL_HOR_NICE_SLIDER);
		m_distRefrSlider->labelfont(FL_COURIER);
		m_distRefrSlider->labelsize(12);
		m_distRefrSlider->minimum(0);
		m_distRefrSlider->maximum(1);
		m_distRefrSlider->step(1);
		m_distRefrSlider->value(0);
		m_distRefrSlider->align(FL_ALIGN_RIGHT);

		m_distRaysSlider = new Fl_Value_Slider(10, 205, 180, 20, "Dist Rays (n^2)");
		m_distRaysSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_distRaysSlider->type(FL_HOR_NICE_SLIDER);
		m_distRaysSlider->labelfont(FL_COURIER);
		m_distRaysSlider->labelsize(12);
		m_distRaysSlider->minimum(2);
		m_distRaysSlider->maximum(6);
		m_distRaysSlider->step(1);
		m_distRaysSlider->value(2);
		m_distRaysSlider->align(FL_ALIGN_RIGHT);

		m_distSizeSlider = new Fl_Value_Slider(10, 230, 180, 20, "Dist Size");
		m_distSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_distSizeSlider->type(FL_HOR_NICE_SLIDER);
		m_distSizeSlider->labelfont(FL_COURIER);
		m_distSizeSlider->labelsize(12);
		m_distSizeSlider->minimum(0);
		m_distSizeSlider->maximum(0.1f);
		m_distSizeSlider->step(0.001f);
		m_distSizeSlider->value(0.005f);
		m_distSizeSlider->align(FL_ALIGN_RIGHT);

		m_thresholdSlider = new Fl_Value_Slider(10, 255, 180, 20, "Intensity threshold");
		m_thresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_thresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_thresholdSlider->labelfont(FL_COURIER);
		m_thresholdSlider->labelsize(12);
		m_thresholdSlider->minimum(0);
		m_thresholdSlider->maximum(1.0);
		m_thresholdSlider->step(0.01);
		m_thresholdSlider->value(0);
		m_thresholdSlider->align(FL_ALIGN_RIGHT);

		m_softShadowButton = new Fl_Light_Button(10, 280, 120, 20, "Soft Shadow");
		m_softShadowButton->user_data((void*)(this));
		m_softShadowButton->value(0);
		m_softShadowButton->callback(cb_softShadowButton);

		m_renderButton = new Fl_Button(270, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(270, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);
    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}