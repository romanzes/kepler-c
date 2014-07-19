#ifndef IMPORTGL_H_INCLUDED
#define IMPORTGL_H_INCLUDED

#include "GLES/gl.h"

/* Dynamically fetches pointers to the egl & gl functions.
 * Should be called once on application initialization.
 * Returns non-zero on success and 0 on failure.
 */
extern int importGLInit();

/* Frees the handle to egl & gl functions library.
 */
extern void importGLDeinit();

/* Use DISABLE_IMPORTGL if you want to link the OpenGL ES at
 * compile/link time and not import it dynamically runtime.
 */
#ifndef DISABLE_IMPORTGL


#ifndef IMPORTGL_API
#define IMPORTGL_API extern
#endif
#ifndef IMPORTGL_FNPTRINIT
#define IMPORTGL_FNPTRINIT
#endif

#define FNDEF(retType, funcName, args) IMPORTGL_API retType (*funcPtr_##funcName) args IMPORTGL_FNPTRINIT

FNDEF(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
FNDEF(void, glClear, (GLbitfield mask));
FNDEF(void, glClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
FNDEF(void, glClearColorx, (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha));
FNDEF(void, glColor4f, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
FNDEF(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha));
FNDEF(void, glColorPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glDisable, (GLenum cap));
FNDEF(void, glDisableClientState, (GLenum array));
FNDEF(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
FNDEF(void, glEnable, (GLenum cap));
FNDEF(void, glEnableClientState, (GLenum array));
FNDEF(void, glFrustumx, (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar));
FNDEF(GLenum, glGetError, (void));
FNDEF(void, glLightxv, (GLenum light, GLenum pname, const GLfixed *params));
FNDEF(void, glLoadIdentity, (void));
FNDEF(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param));
FNDEF(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed *params));
FNDEF(void, glMatrixMode, (GLenum mode));
FNDEF(void, glMultMatrixx, (const GLfixed *m));
FNDEF(void, glNormalPointer, (GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glOrthof, (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal));
FNDEF(void, glOrthox, (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed nearVal, GLfixed farVal));
FNDEF(void, glPopMatrix, (void));
FNDEF(void, glPushMatrix, (void));
FNDEF(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glScalex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glShadeModel, (GLenum mode));
FNDEF(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));


#undef FN
#define FNPTR(name) funcPtr_##name

#ifndef IMPORTGL_NO_FNPTR_DEFS

// Redirect and gl* function calls to and funcPtr_gl*.

#define glBlendFunc             FNPTR(glBlendFunc)
#define glClear                 FNPTR(glClear)
#define glClearColor			FNPTR(glClearColor)
#define glClearColorx           FNPTR(glClearColorx)
#define glColor4f               FNPTR(glColor4f)
#define glColor4x               FNPTR(glColor4x)
#define glColorPointer          FNPTR(glColorPointer)
#define glDisable               FNPTR(glDisable)
#define glDisableClientState    FNPTR(glDisableClientState)
#define glDrawArrays            FNPTR(glDrawArrays)
#define glEnable                FNPTR(glEnable)
#define glEnableClientState     FNPTR(glEnableClientState)
#define glFrustumx              FNPTR(glFrustumx)
#define glGetError              FNPTR(glGetError)
#define glLightxv               FNPTR(glLightxv)
#define glLoadIdentity          FNPTR(glLoadIdentity)
#define glMaterialx             FNPTR(glMaterialx)
#define glMaterialxv            FNPTR(glMaterialxv)
#define glMatrixMode            FNPTR(glMatrixMode)
#define glMultMatrixx           FNPTR(glMultMatrixx)
#define glNormalPointer         FNPTR(glNormalPointer)
#define glOrthof                FNPTR(glOrthof)
#define glOrthox                FNPTR(glOrthox)
#define glPopMatrix             FNPTR(glPopMatrix)
#define glPushMatrix            FNPTR(glPushMatrix)
#define glRotatex               FNPTR(glRotatex)
#define glScalex                FNPTR(glScalex)
#define glShadeModel            FNPTR(glShadeModel)
#define glTranslatex            FNPTR(glTranslatex)
#define glVertexPointer         FNPTR(glVertexPointer)
#define glViewport              FNPTR(glViewport)

#endif // !IMPORTGL_NO_FNPTR_DEFS


#endif // !DISABLE_IMPORTGL


#endif // !IMPORTGL_H_INCLUDED
