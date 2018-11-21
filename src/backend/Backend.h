#ifndef __BACKEND_H__
#define __BACKEND_H__

#include <iostream>
#include <string>
#include <sstream>
#include <exception>

#include <v8.h>
#include <nan.h>
#include <cairo.h>

#include "../dll_visibility.h"

class Canvas;

using namespace std;

class Backend : public Nan::ObjectWrap
{
  private:
    const string name;
    const char* error = NULL;

  protected:
    double width;
    double height;
    cairo_surface_t* surface;
    Canvas* canvas;

    Backend(string name, double width, double height);
    static void init(const Nan::FunctionCallbackInfo<v8::Value> &info);
    static Backend *construct(double width, double height){ return nullptr; }

  public:
    virtual ~Backend();

    void setCanvas(Canvas* canvas);

    virtual cairo_surface_t* createSurface() = 0;
    virtual cairo_surface_t* recreateSurface();

    DLL_PUBLIC cairo_surface_t* getSurface();
    void             destroySurface();

    DLL_PUBLIC string getName();

    virtual double getWidth() {
      return this->width;
    }
    virtual void setWidth(double width_) {
      this->width = width_;
      this->recreateSurface();
    }

    virtual double getHeight() {
      return this->height;
    }
    virtual void setHeight(double height_) {
      this->height = height_;
      this->recreateSurface();
    }

    // Overridden by ImageBackend. SVG and PDF thus always return INVALID.
    virtual cairo_format_t getFormat() {
      return CAIRO_FORMAT_INVALID;
    }

    bool isSurfaceValid();
    inline const char* getError(){ return error; }
};


class BackendOperationNotAvailable: public exception
{
  private:
    Backend* backend;
    string operation_name;

  public:
    BackendOperationNotAvailable(Backend* backend, string operation_name);
    ~BackendOperationNotAvailable() throw();

    const char* what() const throw();
};

#endif
