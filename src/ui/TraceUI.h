//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider*			m_thresholdSlider;
	Fl_Slider*			m_supersamplingSlider;
	Fl_Slider*			m_jitterSlider;
	Fl_Slider*			m_adaptiveSlider;
	Fl_Slider*			m_distReflSlider;
	Fl_Slider*			m_distRefrSlider;
	Fl_Slider*			m_distSizeSlider;
	Fl_Slider*			m_distRaysSlider;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;

	Fl_Light_Button*	m_softShadowButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();
	int			getSuperSampling();
	int			getJitter();
	int			getAdaptiveDepth();
	bool		getSoftShadow();

private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	int			m_nSupersampling;
	int			m_nJitter;
	int			m_nAdaptiveDepth;
	bool		m_is_enable_soft_shadow;


// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_load_background(Fl_Menu_* o, void* v);
	static void cb_load_texture(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);

	static void cb_supersamplingSlides(Fl_Widget* o, void* v);
	static void cb_jitterSlides(Fl_Widget* o, void* v);
	static void cb_adaptiveSlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
	static void cb_softShadowButton(Fl_Widget *o, void*);
};

#endif
