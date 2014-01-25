// NOTE: This must appear before Windows.h in order to avoid build errors.
#include "RubyUtils/RubyUtils.h"

#include <assert.h>
#include <Windows.h>

#include "TaskbarProgress.h"


static HWND get_sketchup_handle();
static Win7TaskbarProgress* get_progressbar(VALUE self);
static void wrap_progressbar_free(Win7TaskbarProgress* progressbar);
static VALUE wrap_progressbar_alloc(VALUE klass);
static VALUE wrap_set_state(VALUE self, VALUE flag);
static VALUE wrap_set_value(VALUE self, VALUE completed, VALUE total);
BOOL CALLBACK EnumThreadWndProc(HWND hwnd,LPARAM lParam);


static HWND get_sketchup_handle()
{
	DWORD thread_id = GetCurrentThreadId();
	assert(thread_id);
	HWND sketchup_handle = NULL;
	LPARAM lParam = reinterpret_cast<LPARAM>(&sketchup_handle);
	BOOL bRetVal = EnumThreadWindows(thread_id, EnumThreadWndProc, lParam);
	assert(sketchup_handle);
	return sketchup_handle;
}

BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lParam)
{
	// TODO(thomthom): Might want to check the window title of `root` to ensure
	// that is really is the SketchUp Window. When going over the whole set of
	// windows it appear to be some where the root isn't SketchUp.
	HWND root = GetAncestor(hwnd, GA_ROOTOWNER);
	assert(root);
	assert(lParam);
	HWND* hwnd_out = reinterpret_cast<HWND*>(lParam);
	assert(hwnd_out);
	*hwnd_out = root;
	return FALSE;
}


static Win7TaskbarProgress* get_progressbar(VALUE self) {
  Win7TaskbarProgress* progressbar;
  Data_Get_Struct(self, Win7TaskbarProgress, progressbar);
  return progressbar;
}
 
static void wrap_progressbar_free(Win7TaskbarProgress* progressbar) {
  ruby_xfree(progressbar);
}
 
static VALUE wrap_progressbar_alloc(VALUE klass) {
	Win7TaskbarProgress* progressbar;
  return Data_Make_Struct(klass, Win7TaskbarProgress, NULL,
		wrap_progressbar_free, progressbar);
}


static VALUE wrap_set_state(VALUE self, VALUE v_flag)
{
	int flag_value = NUM2INT(v_flag);
	switch (flag_value)
	{
	case TBPF_NOPROGRESS:
	case TBPF_INDETERMINATE:
	case TBPF_NORMAL:
	case TBPF_ERROR:
	case TBPF_PAUSED:
		// Valid values.
		break;
	default:
		return Qfalse;
	}
	TBPFLAG flag = static_cast<TBPFLAG>(flag_value);
	HWND sketchup_window = get_sketchup_handle();
	Win7TaskbarProgress* progressbar = get_progressbar(self);
	HRESULT result = progressbar->SetProgressState(sketchup_window, flag);
  return LONG2NUM(result);
}


static VALUE wrap_set_value(VALUE self, VALUE v_completed, VALUE v_total)
{
	ULONGLONG completed = NUM2ULL(v_completed);
	ULONGLONG total = NUM2ULL(v_total);
  HWND sketchup = get_sketchup_handle();
	Win7TaskbarProgress* progressbar = get_progressbar(self);
	HRESULT result = progressbar->SetProgressValue(sketchup, completed, total);
  return LONG2NUM(result);
}


extern "C"
void Init_TaskbarProgress()
{
	VALUE cTaskbarProcess = rb_define_class("TaskbarProgress", rb_cObject);

	rb_define_const(cTaskbarProcess, "CEXT_VERSION",  GetRubyInterface("1.0.0"));

	rb_define_alloc_func(cTaskbarProcess, wrap_progressbar_alloc);

	rb_define_method(cTaskbarProcess, "set_state", VALUEFUNC(wrap_set_state), 1);
	rb_define_method(cTaskbarProcess, "set_value", VALUEFUNC(wrap_set_value), 2);

	rb_define_const(cTaskbarProcess, "NOPROGRESS",    INT2NUM(TBPF_NOPROGRESS));
	rb_define_const(cTaskbarProcess, "INDETERMINATE", INT2NUM(TBPF_INDETERMINATE));
	rb_define_const(cTaskbarProcess, "NORMAL",        INT2NUM(TBPF_NORMAL));
	rb_define_const(cTaskbarProcess, "ERROR",         INT2NUM(TBPF_ERROR));
	rb_define_const(cTaskbarProcess, "PAUSED",        INT2NUM(TBPF_PAUSED));
}
