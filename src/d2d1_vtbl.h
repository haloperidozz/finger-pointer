/***********************************************************************
 * HACK: Microsoft's official d2d1.h header file d2d1.h has no vtbl
 * definitions, making it impossible to use in C.
 * 
 * Structure definitions and macros are taken from the d2d1.h header file,
 * which is part of the mingw-w64 runtime package.
 * 
 * https://github.com/mingw-w64/mingw-w64/blob/master/mingw-w64-headers/include/d2d1.h
 ***********************************************************************/

#ifndef __D2D1_VTBL_H
#define __D2D1_VTBL_H

#include <Unknwnbase.h>
#include <d2d1.h>

#if _MSC_VER /* MSVC */

/***********************************************************************
 * ID2D1Resource
 ***********************************************************************/

typedef struct ID2D1ResourceVtbl {
    IUnknownVtbl Base;

    STDMETHOD_(void, GetFactory)(ID2D1Resource *This, ID2D1Factory **factory) PURE;
} ID2D1ResourceVtbl;

interface ID2D1Resource {
    const ID2D1ResourceVtbl *lpVtbl;
};

#define ID2D1Resource_QueryInterface(this,A,B) (this)->lpVtbl->Base.QueryInterface((IUnknown*)(this),A,B)
#define ID2D1Resource_AddRef(this) (this)->lpVtbl->Base.AddRef((IUnknown*)(this))
#define ID2D1Resource_Release(this) (this)->lpVtbl->Base.Release((IUnknown*)(this))
#define ID2D1Resource_GetFactory(this,A) (this)->lpVtbl->GetFactory(this,A)

/***********************************************************************
 * ID2D1Factory
 ***********************************************************************/

typedef struct ID2D1FactoryVtbl {
    IUnknownVtbl Base;

    STDMETHOD(ReloadSystemMetrics)(ID2D1Factory *This) PURE;
    STDMETHOD_(void, GetDesktopDpi)(ID2D1Factory *This, FLOAT *dpiX, FLOAT *dpiY) PURE;
    STDMETHOD(CreateRectangleGeometry)(ID2D1Factory *This, const D2D1_RECT_F *rectangle, ID2D1RectangleGeometry **rectangleGeometry) PURE;
    STDMETHOD(CreateRoundedRectangleGeometry)(ID2D1Factory *This, const D2D1_ROUNDED_RECT *roundedRectangle, ID2D1RoundedRectangleGeometry **roundedRectangleGeometry) PURE;
    STDMETHOD(CreateEllipseGeometry)(ID2D1Factory *This, const D2D1_ELLIPSE *ellipse, ID2D1EllipseGeometry **ellipseGeometry) PURE;
    STDMETHOD(CreateGeometryGroup)(ID2D1Factory *This, D2D1_FILL_MODE fillMode, ID2D1Geometry **geometries, UINT geometriesCount, ID2D1GeometryGroup **geometryGroup) PURE;
    STDMETHOD(CreateTransformedGeometry)(ID2D1Factory *This, ID2D1Geometry *sourceGeometry, const D2D1_MATRIX_3X2_F *transform, ID2D1TransformedGeometry **transformedGeometry) PURE;
    STDMETHOD(CreatePathGeometry)(ID2D1Factory *This, ID2D1PathGeometry **pathGeometry) PURE;
    STDMETHOD(CreateStrokeStyle)(ID2D1Factory *This, const D2D1_STROKE_STYLE_PROPERTIES *strokeStyleProperties, const FLOAT *dashes, UINT dashesCount, ID2D1StrokeStyle **strokeStyle) PURE;
    STDMETHOD(CreateDrawingStateBlock)(ID2D1Factory *This, const D2D1_DRAWING_STATE_DESCRIPTION *drawingStateDescription, IDWriteRenderingParams *textRenderingParams, ID2D1DrawingStateBlock **drawingStateBlock) PURE;
    STDMETHOD(CreateWicBitmapRenderTarget)(ID2D1Factory *This, IWICBitmap *target, const D2D1_RENDER_TARGET_PROPERTIES *renderTargetProperties, ID2D1RenderTarget **renderTarget) PURE;
    STDMETHOD(CreateHwndRenderTarget)(ID2D1Factory *This, const D2D1_RENDER_TARGET_PROPERTIES *renderTargetProperties, const D2D1_HWND_RENDER_TARGET_PROPERTIES *hwndRenderTargetProperties, ID2D1HwndRenderTarget **hwndRenderTarget) PURE;
    STDMETHOD(CreateDxgiSurfaceRenderTarget)(ID2D1Factory *This, IDXGISurface *dxgiSurface, const D2D1_RENDER_TARGET_PROPERTIES *renderTargetProperties, ID2D1RenderTarget **renderTarget) PURE;
    STDMETHOD(CreateDCRenderTarget)(ID2D1Factory *This, const D2D1_RENDER_TARGET_PROPERTIES *renderTargetProperties, ID2D1DCRenderTarget **dcRenderTarget) PURE;
} ID2D1FactoryVtbl;

interface ID2D1Factory {
    const ID2D1FactoryVtbl *lpVtbl;
};

#define ID2D1Factory_QueryInterface(this,A,B) (this)->lpVtbl->Base.QueryInterface((IUnknown*)(this),A,B)
#define ID2D1Factory_AddRef(this) (this)->lpVtbl->Base.AddRef((IUnknown*)(this))
#define ID2D1Factory_Release(this) (this)->lpVtbl->Base.Release((IUnknown*)(this))
#define ID2D1Factory_CreateDCRenderTarget(this,A,B) (this)->lpVtbl->CreateDCRenderTarget(this,A,B)
#define ID2D1Factory_CreateDrawingStateBlock(this,A,B,C) (this)->lpVtbl->CreateDrawingStateBlock(this,A,B,C)
#define ID2D1Factory_CreateDxgiSurfaceRenderTarget(this,A,B,C) (this)->lpVtbl->CreateDxgiSurfaceRenderTarget(this,A,B,C)
#define ID2D1Factory_CreateEllipseGeometry(this,A,B) (this)->lpVtbl->CreateEllipseGeometry(this,A,B)
#define ID2D1Factory_CreateGeometryGroup(this,A,B,C,D) (this)->lpVtbl->CreateGeometryGroup(this,A,B,C,D)
#define ID2D1Factory_CreateHwndRenderTarget(this,A,B,C) (this)->lpVtbl->CreateHwndRenderTarget(this,A,B,C)
#define ID2D1Factory_CreatePathGeometry(this,A) (this)->lpVtbl->CreatePathGeometry(this,A)
#define ID2D1Factory_CreateRectangleGeometry(this,A,B) (this)->lpVtbl->CreateRectangleGeometry(this,A,B)
#define ID2D1Factory_CreateRoundedRectangleGeometry(this,A,B) (this)->lpVtbl->CreateRoundedRectangleGeometry(this,A,B)
#define ID2D1Factory_CreateStrokeStyle(this,A,B,C,D) (this)->lpVtbl->CreateStrokeStyle(this,A,B,C,D)
#define ID2D1Factory_CreateTransformedGeometry(this,A,B,C) (this)->lpVtbl->CreateTransformedGeometry(this,A,B,C)
#define ID2D1Factory_CreateWicBitmapRenderTarget(this,A,B,C) (this)->lpVtbl->CreateWicBitmapRenderTarget(this,A,B,C)
#define ID2D1Factory_GetDesktopDpi(this,A,B) (this)->lpVtbl->GetDesktopDpi(this,A,B)
#define ID2D1Factory_ReloadSystemMetrics(this) (this)->lpVtbl->ReloadSystemMetrics(this)

/***********************************************************************
 * ID2D1Image
 ***********************************************************************/

typedef struct ID2D1ImageVtbl {
    ID2D1ResourceVtbl Base;
} ID2D1ImageVtbl;

interface ID2D1Image {
    const ID2D1ImageVtbl *lpVtbl;
};

#define ID2D1Image_QueryInterface(this,A,B) (this)->lpVtbl->Base.Base.QueryInterface((IUnkwnown*)(this),A,B)
#define ID2D1Image_AddRef(this) (this)->lpVtbl->Base.Base.AddRef((IUnknown*)(this))
#define ID2D1Image_Release(this) (this)->lpVtbl->Base.Base.Release((IUnknown*)(this))
#define ID2D1Image_GetFactory(this,A) (this)->lpVtbl->Base.GetFactory((ID2D1Resource*)(this),A)

/***********************************************************************
 * ID2D1Bitmap
 ***********************************************************************/

typedef struct ID2D1BitmapVtbl {
    ID2D1ImageVtbl Base;

    STDMETHOD_(D2D1_SIZE_F, GetSize)(ID2D1Bitmap *This) PURE;
    STDMETHOD_(D2D1_SIZE_U, GetPixelSize)(ID2D1Bitmap *This) PURE;
    STDMETHOD_(D2D1_PIXEL_FORMAT, GetPixelFormat)(ID2D1Bitmap *This) PURE;
    STDMETHOD_(void, GetDpi)(ID2D1Bitmap *This, FLOAT *dpiX, FLOAT *dpiY) PURE;
    STDMETHOD(CopyFromBitmap)(ID2D1Bitmap *This, const D2D1_POINT_2U *destPoint, ID2D1Bitmap *bitmap, const D2D1_RECT_U *srcRect) PURE;
    STDMETHOD(CopyFromRenderTarget)(ID2D1Bitmap *This, const D2D1_POINT_2U *destPoint, ID2D1RenderTarget *renderTarget, const D2D1_RECT_U *srcRect) PURE;
    STDMETHOD(CopyFromMemory)(ID2D1Bitmap *This, const D2D1_RECT_U *dstRect, const void *srcData, UINT32 pitch) PURE;
} ID2D1BitmapVtbl;

interface ID2D1Bitmap {
    const ID2D1BitmapVtbl *lpVtbl;
};

#define ID2D1Bitmap_QueryInterface(this,A,B) (this)->lpVtbl->Base.Base.Base.QueryInterface((IUnkwnown*)(this),A,B)
#define ID2D1Bitmap_AddRef(this) (this)->lpVtbl->Base.Base.Base.AddRef((IUnknown*)(this))
#define ID2D1Bitmap_Release(this) (this)->lpVtbl->Base.Base.Base.Release((IUnknown*)(this))
#define ID2D1Bitmap_GetFactory(this,A) (this)->lpVtbl->Base.Base.GetFactory((ID2D1Resource*)(this),A)
#define ID2D1Bitmap_GetSize(this) (this)->lpVtbl->GetSize(this)
#define ID2D1Bitmap_GetPixelSize(this) (this)->lpVtbl->GetPixelSize(this)
#define ID2D1Bitmap_GetPixelFormat(this) (this)->lpVtbl->GetPixelFormat(this)
#define ID2D1Bitmap_GetDpi(this,A,B) (this)->lpVtbl->GetDpi(this,A,B)
#define ID2D1Bitmap_CopyFromBitmap(this,A,B,C) (this)->lpVtbl->CopyFromBitmap(this,A,B,C)
#define ID2D1Bitmap_CopyFromRenderTarget(this,A,B,C) (this)->lpVtbl->CopyFromRenderTarget(this,A,B,C)
#define ID2D1Bitmap_CopyFromMemory(this,A,B,C) (this)->lpVtbl->CopyFromMemory(this,A,B,C)

/***********************************************************************
 * ID2D1RenderTarget
 ***********************************************************************/

typedef struct ID2D1RenderTargetVtbl {
    ID2D1ResourceVtbl Base;

    STDMETHOD(CreateBitmap)(ID2D1RenderTarget *This, D2D1_SIZE_U size, const void *srcData, UINT32 pitch, const D2D1_BITMAP_PROPERTIES *bitmapProperties, ID2D1Bitmap **bitmap) PURE;
    STDMETHOD(CreateBitmapFromWicBitmap)(ID2D1RenderTarget *This, IWICBitmapSource *wicBitmapSource, const D2D1_BITMAP_PROPERTIES *bitmapProperties, ID2D1Bitmap **bitmap) PURE;
    STDMETHOD(CreateSharedBitmap)(ID2D1RenderTarget *This, REFIID riid, void *data, const D2D1_BITMAP_PROPERTIES *bitmapProperties, ID2D1Bitmap **bitmap) PURE;
    STDMETHOD(CreateBitmapBrush)(ID2D1RenderTarget *This, ID2D1Bitmap *bitmap, const D2D1_BITMAP_BRUSH_PROPERTIES *bitmapBrushProperties, const D2D1_BRUSH_PROPERTIES *brushProperties, ID2D1BitmapBrush **bitmapBrush) PURE;
    STDMETHOD(CreateSolidColorBrush)(ID2D1RenderTarget *This, const D2D1_COLOR_F *color, const D2D1_BRUSH_PROPERTIES *brushProperties, ID2D1SolidColorBrush **solidColorBrush) PURE;
    STDMETHOD(CreateGradientStopCollection)(ID2D1RenderTarget *This, const D2D1_GRADIENT_STOP *gradientStops, UINT gradientStopsCount, D2D1_GAMMA colorInterpolationGamma, D2D1_EXTEND_MODE extendMode, ID2D1GradientStopCollection **gradientStopCollection) PURE;
    STDMETHOD(CreateLinearGradientBrush)(ID2D1RenderTarget *This, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES *linearGradientBrushProperties, const D2D1_BRUSH_PROPERTIES *brushProperties, ID2D1GradientStopCollection *gradientStopCollection, ID2D1LinearGradientBrush **linearGradientBrush) PURE;
    STDMETHOD(CreateRadialGradientBrush)(ID2D1RenderTarget *This, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES *radialGradientBrushProperties, const D2D1_BRUSH_PROPERTIES *brushProperties, ID2D1GradientStopCollection *gradientStopCollection, ID2D1RadialGradientBrush **radialGradientBrush) PURE;
    STDMETHOD(CreateCompatibleRenderTarget)(ID2D1RenderTarget *This, const D2D1_SIZE_F *desiredSize, const D2D1_SIZE_U *desiredPixelSize, const D2D1_PIXEL_FORMAT *desiredFormat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options, ID2D1BitmapRenderTarget **bitmapRenderTarget) PURE;
    STDMETHOD(CreateLayer)(ID2D1RenderTarget *This, const D2D1_SIZE_F *size, ID2D1Layer **layer) PURE;
    STDMETHOD(CreateMesh)(ID2D1RenderTarget *This, ID2D1Mesh **mesh) PURE;
    STDMETHOD_(void, DrawLine)(ID2D1RenderTarget *This, D2D1_POINT_2F point0, D2D1_POINT_2F point1, ID2D1Brush *brush, FLOAT strokeWidth, ID2D1StrokeStyle *strokeStyle) PURE;
    STDMETHOD_(void, DrawRectangle)(ID2D1RenderTarget *This, const D2D1_RECT_F *rect, ID2D1Brush *brush, FLOAT strokeWidth, ID2D1StrokeStyle *strokeStyle) PURE;
    STDMETHOD_(void, FillRectangle)(ID2D1RenderTarget *This, const D2D1_RECT_F *rect, ID2D1Brush *brush) PURE;
    STDMETHOD_(void, DrawRoundedRectangle)(ID2D1RenderTarget *This, const D2D1_ROUNDED_RECT *roundedRect, ID2D1Brush *brush, FLOAT strokeWidth, ID2D1StrokeStyle *strokeStyle) PURE;
    STDMETHOD_(void, FillRoundedRectangle)(ID2D1RenderTarget *This, const D2D1_ROUNDED_RECT *roundedRect, ID2D1Brush *brush) PURE;
    STDMETHOD_(void, DrawEllipse)(ID2D1RenderTarget *This, const D2D1_ELLIPSE *ellipse, ID2D1Brush *brush, FLOAT strokeWidth, ID2D1StrokeStyle *strokeStyle) PURE;
    STDMETHOD_(void, FillEllipse)(ID2D1RenderTarget *This, const D2D1_ELLIPSE *ellipse, ID2D1Brush *brush) PURE;
    STDMETHOD_(void, DrawGeometry)(ID2D1RenderTarget *This, ID2D1Geometry *geometry, ID2D1Brush *brush, FLOAT strokeWidth, ID2D1StrokeStyle *strokeStyle) PURE;
    STDMETHOD_(void, FillGeometry)(ID2D1RenderTarget *This, ID2D1Geometry *geometry, ID2D1Brush *brush, ID2D1Brush *opacityBrush) PURE;
    STDMETHOD_(void, FillMesh)(ID2D1RenderTarget *This, ID2D1Mesh *mesh, ID2D1Brush *brush) PURE;
    STDMETHOD_(void, FillOpacityMask)(ID2D1RenderTarget *This, ID2D1Bitmap *opacityMask, ID2D1Brush *brush, D2D1_OPACITY_MASK_CONTENT content, const D2D1_RECT_F *destinationRectangle, const D2D1_RECT_F *sourceRectangle) PURE;
    STDMETHOD_(void, DrawBitmap)(ID2D1RenderTarget *This, ID2D1Bitmap *bitmap, const D2D1_RECT_F *destinationRectangle, FLOAT opacity, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode, const D2D1_RECT_F *sourceRectangle) PURE;
    STDMETHOD_(void, DrawText)(ID2D1RenderTarget *This, const WCHAR *string, UINT stringLength, IDWriteTextFormat *textFormat, const D2D1_RECT_F *layoutRect, ID2D1Brush *defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options, DWRITE_MEASURING_MODE measuringMode) PURE;
    STDMETHOD_(void, DrawTextLayout)(ID2D1RenderTarget *This, D2D1_POINT_2F origin, IDWriteTextLayout *textLayout, ID2D1Brush *defaultForegroundBrush, D2D1_DRAW_TEXT_OPTIONS options) PURE;
    STDMETHOD_(void, DrawGlyphRun)(ID2D1RenderTarget *This, D2D1_POINT_2F baselineOrigin, const DWRITE_GLYPH_RUN *glyphRun, ID2D1Brush *foregroundBrush, DWRITE_MEASURING_MODE measuringMode) PURE;
    STDMETHOD_(void, SetTransform)(ID2D1RenderTarget *This, const D2D1_MATRIX_3X2_F *transform) PURE;
    STDMETHOD_(void, GetTransform)(ID2D1RenderTarget *This, D2D1_MATRIX_3X2_F *transform) PURE;
    STDMETHOD_(void, SetAntialiasMode)(ID2D1RenderTarget *This, D2D1_ANTIALIAS_MODE antialiasMode) PURE;
    STDMETHOD_(D2D1_ANTIALIAS_MODE, GetAntialiasMode)(ID2D1RenderTarget *This) PURE;
    STDMETHOD_(void, SetTextAntialiasMode)(ID2D1RenderTarget *This, D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode) PURE;
    STDMETHOD_(D2D1_TEXT_ANTIALIAS_MODE, GetTextAntialiasMode)(ID2D1RenderTarget *This) PURE;
    STDMETHOD_(void, SetTextRenderingParams)(ID2D1RenderTarget *This, IDWriteRenderingParams *textRenderingParams) PURE;
    STDMETHOD_(void, GetTextRenderingParams)(ID2D1RenderTarget *This, IDWriteRenderingParams **textRenderingParams) PURE;
    STDMETHOD_(void, SetTags)(ID2D1RenderTarget *This, D2D1_TAG tag1, D2D1_TAG tag2) PURE;
    STDMETHOD_(void, GetTags)(ID2D1RenderTarget *This, D2D1_TAG *tag1, D2D1_TAG *tag2) PURE;
    STDMETHOD_(void, PushLayer)(ID2D1RenderTarget *This, const D2D1_LAYER_PARAMETERS *layerParameters, ID2D1Layer *layer) PURE;
    STDMETHOD_(void, PopLayer)(ID2D1RenderTarget *This) PURE;
    STDMETHOD(Flush)(ID2D1RenderTarget *This, D2D1_TAG *tag1, D2D1_TAG *tag2) PURE;
    STDMETHOD_(void, SaveDrawingState)(ID2D1RenderTarget *This, ID2D1DrawingStateBlock *drawingStateBlock) PURE;
    STDMETHOD_(void, RestoreDrawingState)(ID2D1RenderTarget *This, ID2D1DrawingStateBlock *drawingStateBlock) PURE;
    STDMETHOD_(void, PushAxisAlignedClip)(ID2D1RenderTarget *This, const D2D1_RECT_F *clipRect, D2D1_ANTIALIAS_MODE antialiasMode) PURE;
    STDMETHOD_(void, PopAxisAlignedClip)(ID2D1RenderTarget *This) PURE;
    STDMETHOD_(void, Clear)(ID2D1RenderTarget *This, const D2D1_COLOR_F *clearColor) PURE;
    STDMETHOD_(void, BeginDraw)(ID2D1RenderTarget *This) PURE;
    STDMETHOD(EndDraw)(ID2D1RenderTarget *This, D2D1_TAG *tag1, D2D1_TAG *tag2) PURE;
    STDMETHOD_(D2D1_PIXEL_FORMAT, GetPixelFormat)(ID2D1RenderTarget *This) PURE;
    STDMETHOD_(void, SetDpi)(ID2D1RenderTarget *This, FLOAT dpiX, FLOAT dpiY) PURE;
    STDMETHOD_(void, GetDpi)(ID2D1RenderTarget *This, FLOAT *dpiX, FLOAT *dpiY) PURE;
    STDMETHOD_(D2D1_SIZE_F, GetSize)(ID2D1RenderTarget *This) PURE;
    STDMETHOD_(D2D1_SIZE_U, GetPixelSize)(ID2D1RenderTarget *This) PURE;
    STDMETHOD_(UINT32, GetMaximumBitmapSize)(ID2D1RenderTarget *This) PURE;
    STDMETHOD_(BOOL, IsSupported)(ID2D1RenderTarget *This, const D2D1_RENDER_TARGET_PROPERTIES *renderTargetProperties) PURE;
} ID2D1RenderTargetVtbl;

interface ID2D1RenderTarget {
    const ID2D1RenderTargetVtbl *lpVtbl;
};

#define ID2D1RenderTarget_QueryInterface(this,A,B) (this)->lpVtbl->Base.Base.QueryInterface((IUnknown*)(this),A,B)
#define ID2D1RenderTarget_AddRef(this) (this)->lpVtbl->Base.Base.AddRef((IUnknown*)(this))
#define ID2D1RenderTarget_Release(this) (this)->lpVtbl->Base.Base.Release((IUnknown*)(this))
#define ID2D1RenderTarget_GetFactory(this,A) (this)->lpVtbl->Base.GetFactory((ID2D1Resource*)(this),A)
#define ID2D1RenderTarget_BeginDraw(this) (this)->lpVtbl->BeginDraw(this)
#define ID2D1RenderTarget_Clear(this,A) (this)->lpVtbl->Clear(this,A)
#define ID2D1RenderTarget_CreateBitmap(this,A,B,C,D,E) (this)->lpVtbl->CreateBitmap(this,A,B,C,D,E)
#define ID2D1RenderTarget_CreateBitmapBrush(this,A,B) (this)->lpVtbl->CreateBitmapBrush(this,A,B)
#define ID2D1RenderTarget_CreateBitmapFromWicBitmap(this,A,B,C) (this)->lpVtbl->CreateBitmapFromWicBitmap(this,A,B,C)
#define ID2D1RenderTarget_CreateCompatibleRenderTarget(this,A,B,C,D,E) (this)->lpVtbl->CreateCompatibleRenderTarget(this,A,B,C,D,E)
#define ID2D1RenderTarget_CreateGradientStopCollection(this,A,B,C) (this)->lpVtbl->CreateGradientStopCollection(this,A,B,C)
#define ID2D1RenderTarget_CreateLayer(this,A,B) (this)->lpVtbl->CreateLayer(this,A,B)
#define ID2D1RenderTarget_CreateLinearGradientBrush(this,A,B,C,D) (this)->lpVtbl->CreateLinearGradientBrush(this,A,B,C,D)
#define ID2D1RenderTarget_CreateMesh(this,A) (this)->lpVtbl->CreateMesh(this,A)
#define ID2D1RenderTarget_CreateRadialGradientBrush(this,A,B,C,D) (this)->lpVtbl->CreateRadialGradientBrush(this,A,B,C,D)
#define ID2D1RenderTarget_CreateSharedBitmap(this,A,B,C,D) (this)->lpVtbl->CreateSharedBitmap(this,A,B,C,D)
#define ID2D1RenderTarget_CreateSolidColorBrush(this,A,B,C) (this)->lpVtbl->CreateSolidColorBrush(this,A,B,C)
#define ID2D1RenderTarget_DrawBitmap(this,A,B,C,D,E) (this)->lpVtbl->DrawBitmap(this,A,B,C,D,E)
#define ID2D1RenderTarget_DrawEllipse(this,A,B,C,D) (this)->lpVtbl->DrawEllipse(this,A,B,C,D)
#define ID2D1RenderTarget_DrawGeometry(this,A,B,C,D) (this)->lpVtbl->DrawGeometry(this,A,B,C,D)
#define ID2D1RenderTarget_DrawGlyphRun(this,A,B,C,D) (this)->lpVtbl->DrawGlyphRun(this,A,B,C,D)
#define ID2D1RenderTarget_DrawLine(this,A,B,C,D,E) (this)->lpVtbl->DrawLine(this,A,B,C,D,E)
#define ID2D1RenderTarget_DrawRectangle(this,A,B,C,D) (this)->lpVtbl->DrawRectangle(this,A,B,C,D)
#define ID2D1RenderTarget_DrawRoundedRectangle(this,A,B,C,D) (this)->lpVtbl->DrawRoundedRectangle(this,A,B,C,D)
#define ID2D1RenderTarget_DrawText(this,A,B,C,D,E,F,G) (this)->lpVtbl->DrawText(this,A,B,C,D,E,F,G)
#define ID2D1RenderTarget_DrawTextLayout(this,A,B,C,D) (this)->lpVtbl->DrawTextLayout(this,A,B,C,D)
#define ID2D1RenderTarget_EndDraw(this,A,B) (this)->lpVtbl->EndDraw(this,A,B)
#define ID2D1RenderTarget_FillEllipse(this,A,B) (this)->lpVtbl->FillEllipse(this,A,B)
#define ID2D1RenderTarget_FillGeometry(this,A,B,C) (this)->lpVtbl->FillGeometry(this,A,B,C)
#define ID2D1RenderTarget_FillMesh(this,A,B) (this)->lpVtbl->FillMesh(this,A,B)
#define ID2D1RenderTarget_FillOpacityMask(this,A,B,C,D,E) (this)->lpVtbl->FillOpacityMask(this,A,B,C,D,E)
#define ID2D1RenderTarget_FillRectangle(this,A,B) (this)->lpVtbl->FillRectangle(this,A,B)
#define ID2D1RenderTarget_FillRoundedRectangle(this,A,B) (this)->lpVtbl->FillRoundedRectangle(this,A,B)
#define ID2D1RenderTarget_Flush(this,A,B) (this)->lpVtbl->Flush(this,A,B)
#define ID2D1RenderTarget_GetAntialiasMode(this) (this)->lpVtbl->GetAntialiasMode(this)
#define ID2D1RenderTarget_GetDpi(this,A,B) (this)->lpVtbl->GetDpi(this,A,B)
#define ID2D1RenderTarget_GetMaximumBitmapSize(this) (this)->lpVtbl->GetMaximumBitmapSize(this)
#define ID2D1RenderTarget_GetPixelFormat(this) (this)->lpVtbl->GetPixelFormat(this)
#define ID2D1RenderTarget_GetPixelSize(this) (this)->lpVtbl->GetPixelSize(this)
#define ID2D1RenderTarget_GetSize(this) (this)->lpVtbl->GetSize(this)
#define ID2D1RenderTarget_GetTags(this,A,B) (this)->lpVtbl->GetTags(this,A,B)
#define ID2D1RenderTarget_GetTextAntialiasMode(this) (this)->lpVtbl->GetTextAntialiasMode(this)
#define ID2D1RenderTarget_GetTextRenderingParams(this,A) (this)->lpVtbl->GetTextRenderingParams(this,A)
#define ID2D1RenderTarget_GetTransform(this,A) (this)->lpVtbl->GetTransform(this,A)
#define ID2D1RenderTarget_IsSupported(this,A) (this)->lpVtbl->IsSupported(this,A)
#define ID2D1RenderTarget_PopAxisAlignedClip(this) (this)->lpVtbl->PopAxisAlignedClip(this)
#define ID2D1RenderTarget_PopLayer(this) (this)->lpVtbl->PopLayer(this)
#define ID2D1RenderTarget_PushAxisAlignedClip(this,A,B) (this)->lpVtbl->PushAxisAlignedClip(this,A,B)
#define ID2D1RenderTarget_PushLayer(this,A,B) (this)->lpVtbl->PushLayer(this,A,B)
#define ID2D1RenderTarget_RestoreDrawingState(this,A) (this)->lpVtbl->RestoreDrawingState(this,A)
#define ID2D1RenderTarget_SaveDrawingState(this,A) (this)->lpVtbl->SaveDrawingState(this,A)
#define ID2D1RenderTarget_SetAntialiasMode(this,A) (this)->lpVtbl->SetAntialiasMode(this,A)
#define ID2D1RenderTarget_SetDpi(this,A,B) (this)->lpVtbl->SetDpi(this,A,B)
#define ID2D1RenderTarget_SetTags(this,A,B) (this)->lpVtbl->SetTags(this,A,B)
#define ID2D1RenderTarget_SetTextAntialiasMode(this,A) (this)->lpVtbl->SetTextAntialiasMode(this,A)
#define ID2D1RenderTarget_SetTextRenderingParams(this,A) (this)->lpVtbl->SetTextRenderingParams(this,A)
#define ID2D1RenderTarget_SetTransform(this,A) (this)->lpVtbl->SetTransform(this,A)

/***********************************************************************
 * ID2D1HwndRenderTarget
 ***********************************************************************/

typedef interface ID2D1HwndRenderTarget ID2D1HwndRenderTarget;

typedef struct ID2D1HwndRenderTargetVtbl {
    ID2D1RenderTargetVtbl Base;

    STDMETHOD_(D2D1_WINDOW_STATE, CheckWindowState)(ID2D1HwndRenderTarget *This) PURE;
    STDMETHOD(Resize)(ID2D1HwndRenderTarget *This, const D2D1_SIZE_U *pixelSize) PURE;
    STDMETHOD_(HWND, GetHwnd)(ID2D1HwndRenderTarget *This) PURE;
} ID2D1HwndRenderTargetVtbl;

interface ID2D1HwndRenderTarget {
    const struct ID2D1HwndRenderTargetVtbl *lpVtbl;
};

#define ID2D1HwndRenderTarget_QueryInterface(this,A,B) (this)->lpVtbl->Base.Base.Base.QueryInterface((IUnknown*)(this),A,B)
#define ID2D1HwndRenderTarget_AddRef(this) (this)->lpVtbl->Base.Base.Base.AddRef((IUnknown*)(this))
#define ID2D1HwndRenderTarget_Release(this) (this)->lpVtbl->Base.Base.Base.Release((IUnknown*)(this))
#define ID2D1HwndRenderTarget_GetFactory(this,A) (this)->lpVtbl->Base.Base.GetFactory((ID2D1Resource*)(this),A)
#define ID2D1HwndRenderTarget_BeginDraw(this) (this)->lpVtbl->Base.BeginDraw((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_Clear(this,A) (this)->lpVtbl->Base.Clear((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_CreateBitmap(this,A,B,C,D,E) (this)->lpVtbl->Base.CreateBitmap((ID2D1RenderTarget*)(this),A,B,C,D,E)
#define ID2D1HwndRenderTarget_CreateBitmapBrush(this,A,B) (this)->lpVtbl->Base.CreateBitmapBrush((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_CreateBitmapFromWicBitmap(this,A,B,C) (this)->lpVtbl->Base.CreateBitmapFromWicBitmap((ID2D1RenderTarget*)(this),A,B,C)
#define ID2D1HwndRenderTarget_CreateCompatibleRenderTarget(this,A,B,C,D,E) (this)->lpVtbl->Base.CreateCompatibleRenderTarget((ID2D1RenderTarget*)(this),A,B,C,D,E)
#define ID2D1HwndRenderTarget_CreateGradientStopCollection(this,A,B,C) (this)->lpVtbl->Base.CreateGradientStopCollection((ID2D1RenderTarget*)(this),A,B,C)
#define ID2D1HwndRenderTarget_CreateLayer(this,A,B) (this)->lpVtbl->Base.CreateLayer((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_CreateLinearGradientBrush(this,A,B,C,D) (this)->lpVtbl->Base.CreateLinearGradientBrush((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_CreateMesh(this,A) (this)->lpVtbl->Base.CreateMesh((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_CreateRadialGradientBrush(this,A,B,C,D) (this)->lpVtbl->Base.CreateRadialGradientBrush((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_CreateSharedBitmap(this,A,B,C,D) (this)->lpVtbl->Base.CreateSharedBitmap((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_CreateSolidColorBrush(this,A,B,C) (this)->lpVtbl->Base.CreateSolidColorBrush((ID2D1RenderTarget*)(this),A,B,C)
#define ID2D1HwndRenderTarget_DrawBitmap(this,A,B,C,D,E) (this)->lpVtbl->Base.DrawBitmap((ID2D1RenderTarget*)(this),A,B,C,D,E)
#define ID2D1HwndRenderTarget_DrawEllipse(this,A,B,C,D) (this)->lpVtbl->Base.DrawEllipse((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_DrawGeometry(this,A,B,C,D) (this)->lpVtbl->Base.DrawGeometry((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_DrawGlyphRun(this,A,B,C,D) (this)->lpVtbl->Base.DrawGlyphRun((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_DrawLine(this,A,B,C,D,E) (this)->lpVtbl->Base.DrawLine((ID2D1RenderTarget*)(this),A,B,C,D,E)
#define ID2D1HwndRenderTarget_DrawRectangle(this,A,B,C,D) (this)->lpVtbl->Base.DrawRectangle((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_DrawRoundedRectangle(this,A,B,C,D) (this)->lpVtbl->Base.DrawRoundedRectangle((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_DrawText(this,A,B,C,D,E,F,G) (this)->lpVtbl->Base.DrawText((ID2D1RenderTarget*)(this),A,B,C,D,E,F,G)
#define ID2D1HwndRenderTarget_DrawTextLayout(this,A,B,C,D) (this)->lpVtbl->Base.DrawTextLayout((ID2D1RenderTarget*)(this),A,B,C,D)
#define ID2D1HwndRenderTarget_EndDraw(this,A,B) (this)->lpVtbl->Base.EndDraw((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_FillEllipse(this,A,B) (this)->lpVtbl->Base.FillEllipse((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_FillGeometry(this,A,B,C) (this)->lpVtbl->Base.FillGeometry((ID2D1RenderTarget*)(this),A,B,C)
#define ID2D1HwndRenderTarget_FillMesh(this,A,B) (this)->lpVtbl->Base.FillMesh((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_FillOpacityMask(this,A,B,C,D,E) (this)->lpVtbl->Base.FillOpacityMask((ID2D1RenderTarget*)(this),A,B,C,D,E)
#define ID2D1HwndRenderTarget_FillRectangle(this,A,B) (this)->lpVtbl->Base.FillRectangle((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_FillRoundedRectangle(this,A,B) (this)->lpVtbl->Base.FillRoundedRectangle((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_Flush(this,A,B) (this)->lpVtbl->Base.Flush((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_GetAntialiasMode(this) (this)->lpVtbl->Base.GetAntialiasMode((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_GetDpi(this,A,B) (this)->lpVtbl->Base.GetDpi((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_GetMaximumBitmapSize(this) (this)->lpVtbl->Base.GetMaximumBitmapSize((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_GetPixelFormat(this) (this)->lpVtbl->Base.GetPixelFormat((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_GetPixelSize(this) (this)->lpVtbl->Base.GetPixelSize((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_GetSize(this) (this)->lpVtbl->Base.GetSize((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_GetTags(this,A,B) (this)->lpVtbl->Base.GetTags((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_GetTextAntialiasMode(this) (this)->lpVtbl->Base.GetTextAntialiasMode((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_GetTextRenderingParams(this,A) (this)->lpVtbl->Base.GetTextRenderingParams((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_GetTransform(this,A) (this)->lpVtbl->Base.GetTransform((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_IsSupported(this,A) (this)->lpVtbl->Base.IsSupported((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_PopAxisAlignedClip(this) (this)->lpVtbl->Base.PopAxisAlignedClip((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_PopLayer(this) (this)->lpVtbl->Base.PopLayer((ID2D1RenderTarget*)(this))
#define ID2D1HwndRenderTarget_PushAxisAlignedClip(this,A,B) (this)->lpVtbl->Base.PushAxisAlignedClip((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_PushLayer(this,A,B) (this)->lpVtbl->Base.PushLayer((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_RestoreDrawingState(this,A) (this)->lpVtbl->Base.RestoreDrawingState((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_SaveDrawingState(this,A) (this)->lpVtbl->Base.SaveDrawingState((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_SetAntialiasMode(this,A) (this)->lpVtbl->Base.SetAntialiasMode((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_SetDpi(this,A,B) (this)->lpVtbl->Base.SetDpi((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_SetTags(this,A,B) (this)->lpVtbl->Base.SetTags((ID2D1RenderTarget*)(this),A,B)
#define ID2D1HwndRenderTarget_SetTextAntialiasMode(this,A) (this)->lpVtbl->Base.SetTextAntialiasMode((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_SetTextRenderingParams(this,A) (this)->lpVtbl->Base.SetTextRenderingParams((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_SetTransform(this,A) (this)->lpVtbl->Base.SetTransform((ID2D1RenderTarget*)(this),A)
#define ID2D1HwndRenderTarget_CheckWindowState(this) (this)->lpVtbl->CheckWindowState(this)
#define ID2D1HwndRenderTarget_GetHwnd(this) (this)->lpVtbl->GetHwnd(this)
#define ID2D1HwndRenderTarget_Resize(this,A) (this)->lpVtbl->Resize(this,A)

/***********************************************************************/

#endif /* _MSC_VER */

#endif /* __D2D1_VTBL_H */