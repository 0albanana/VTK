#!/usr/local/bin/python

from libVTKCommonPython import *
from libVTKGraphicsPython import *

#catch  load vtktcl 
# create ice-cream cone
# get the interactor ui
#source ../../examplesTcl/vtkInt.tcl
#source ../../examplesTcl/colors.tcl
from colors import *
ren = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# create implicit function primitives
cone = vtkCone()
cone.SetAngle(20)
vertPlane = vtkPlane()
vertPlane.SetOrigin(.1,0,0)
vertPlane.SetNormal(-1,0,0)
basePlane = vtkPlane()
basePlane.SetOrigin(1.2,0,0)
basePlane.SetNormal(1,0,0)
iceCream = vtkSphere()
iceCream.SetCenter(1.333,0,0)
iceCream.SetRadius(0.5)
bite = vtkSphere()
bite.SetCenter(1.5,0,0.5)
bite.SetRadius(0.25)

# combine primitives to build ice-cream cone
theCone = vtkImplicitBoolean()
theCone.SetOperationType(1) #intersection
theCone.AddFunction(cone)
theCone.AddFunction(vertPlane)
theCone.AddFunction(basePlane)

theCream = vtkImplicitBoolean()
theCream.SetOperationType(2) #difference
theCream.AddFunction(iceCream)
theCream.AddFunction(bite)

# iso-surface to create geometry
theConeSample = vtkSampleFunction()
theConeSample.SetImplicitFunction(theCone)
theConeSample.SetModelBounds(-1,1.5,-1.25,1.25,-1.25,1.25)
theConeSample.SetSampleDimensions(60,60,60)
theConeSample.ComputeNormalsOff()
theConeSurface = vtkContourFilter()
theConeSurface.SetInput(theConeSample.GetOutput())
theConeSurface.SetValue(0,0.0)
coneMapper = vtkPolyDataMapper()
coneMapper.SetInput(theConeSurface.GetOutput())
coneMapper.ScalarVisibilityOff()
coneActor = vtkActor()
coneActor.SetMapper(coneMapper)
coneActor.GetProperty().SetColor(chocolate[0],chocolate[1],chocolate[2])

# iso-surface to create geometry
theCreamSample = vtkSampleFunction()
theCreamSample.SetImplicitFunction(theCream)
theCreamSample.SetModelBounds(0,2.5,-1.25,1.25,-1.25,1.25)
theCreamSample.SetSampleDimensions(60,60,60)
theCreamSample.ComputeNormalsOff()
theCreamSurface = vtkContourFilter()
theCreamSurface.SetInput(theCreamSample.GetOutput())
theCreamSurface.SetValue(0,0.0)
creamMapper = vtkPolyDataMapper()
creamMapper.SetInput(theCreamSurface.GetOutput())
creamMapper.ScalarVisibilityOff()
creamActor = vtkActor()
creamActor.SetMapper(creamMapper)
creamActor.GetProperty().SetColor(mint[0],mint[1],mint[2])

# Add the actors to the renderer, set the background and size
#
ren.AddActor(coneActor)
ren.AddActor(creamActor)
ren.SetBackground(1,1,1)
renWin.SetSize(500,500)
ren.GetActiveCamera().Roll(90)
iren.Initialize()

#renWin SetFileName "iceCream.tcl.ppm"
#renWin SaveImageAsPPM

# render the image
#
renWin.Render()

# prevent the tk window from showing up then start the event loop
#wm withdraw .
iren.Start()
