//##########################################################################
//#                                                                        #
//#                       CLOUDCOMPARE PLUGIN: qDummy                      #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                             COPYRIGHT: XXX                             #
//#                                                                        #
//##########################################################################

//First: replace all occurences of 'AdvancedDummyPlugin' by your own plugin class name in this file!
#include "AdvancedDummyPlugin.h"

//Qt
#include <QtGui>

// we want to be able to create point clouds!
#include <ccPointCloud.h>
#include <ccScalarField.h>
#include <ccHObjectCaster.h>

//Default constructor: should mainly be used to initialize
//actions (pointers) and other members
AdvancedDummyPlugin::AdvancedDummyPlugin(QObject* parent/*=0*/)
    : QObject(parent)
    , m_doCreateCloud(0), m_doSomethingOnCloud(0)
{
    // an advanced plugin will provide 1 or more actions, which are physically
    // represented by their own icons in the toolbar
    // here the actions are initialized, but we need to declare them at the *.h file.. see that file
    // to see how to declare a new action.

    // this code here is called once the plugin is created, and creates the actions:
    m_doCreateCloud = new QAction("This action creates a cloud", this);
    m_doCreateCloud->setToolTip("This action is cool! allows you to create a cloud");
    // and an icon here:
    m_doCreateCloud->setIcon(QIcon(":/CC/plugin/AdvancedDummyPlugin/icon.png"));


    // this action will perform some kind of processing on point clouds:
    m_doSomethingOnCloud = new QAction("This action does something on a mesh", this);
    m_doSomethingOnCloud ->setToolTip("we can set the tooltip here also");
    // and an icon also for this one:
    m_doSomethingOnCloud ->setIcon(QIcon(":/CC/plugin/AdvancedDummyPlugin/icon.png"));


    // one an action is triggered (the icon or the menu entry is clicked)
    // CloudCompare must know which plugin's function he need to call.
    // in Qt these are called connections:
    // our action must be connected with some kind of function that will be called:


    // doActionDoSomethingOnCloud function is executed when m_doActionOnCloud is triggered (by clicking on it):
    connect(m_doCreateCloud, SIGNAL(triggered()), this, SLOT(doCreateCloud()));

    // the same for the mesh function
    connect(m_doSomethingOnCloud, SIGNAL(triggered()), this, SLOT(doSomethingOnCloud()));

    // we are ok now! everything was setup
}

//This method should enable or disable each plugin action
//depending on the currently selected entities ('selectedEntities').
//For example: if none of the selected entities is a cloud, and your
//plugin deals only with clouds, call 'm_action->setEnabled(false)'
void AdvancedDummyPlugin::onNewSelection(const ccHObject::Container& selectedEntities)
{
    // the action which creates a new cloud will be always available!
    // we does not need to disable it depending on the selection...


    // the processing function instead will require just one point cloud to be selected:
    if ((selectedEntities.size()) ==1 && (selectedEntities.back()->isA(CC_TYPES::POINT_CLOUD)))
    {// if so, we activate the action:
        m_doSomethingOnCloud->setEnabled(true);
    }
    else // else we disable it
    {
        m_doSomethingOnCloud->setEnabled(false);
    }

    //the same code can be compressed in, this code does exactly the same:
    //m_doSomethingOnCloud->setEnabled((selectedEntities.size()) ==1 && (selectedEntities.back().isA(CC_TYPES::POINT_CLOUD)));
}

//This method returns all 'actions' of your plugin.
//It will be called only once, when plugin is loaded.
// as you might see group is passed by reference so just put your actions there!
void AdvancedDummyPlugin::getActions(QActionGroup& group)
{
    //add the action to the group
    group.addAction(m_doCreateCloud);
    group.addAction(m_doSomethingOnCloud);
}

void AdvancedDummyPlugin::doSomethingOnCloud()
{
    // we get the selected cloud

    // first get ALL selected objects
    const ccHObject::Container& selectedEntities = m_app->getSelectedEntities();

    // transform to a ccPointCloud object
    ccPointCloud* pc = (selectedEntities.size() == 1 ? ccHObjectCaster::ToPointCloud(selectedEntities.back()) : 0);
    if (!pc) // if pc was not created display error message and return!
    {
        m_app->dispToConsole("Select one and only one point cloud!",ccMainAppInterface::ERR_CONSOLE_MESSAGE);
        return;
    }


    // we have our point cloud
    // we can now do whatever we want with it
    // for example we get back the scalar field (if the user did not deleted that)
    if(pc->getNumberOfScalarFields() >= 1)
    {
        // we get the first one!
        // we are casting the value retruned by getScalarField to a true ccScalarField
        ccScalarField * field  = static_cast<ccScalarField *> (pc->getScalarField(0));

        // we also create a new field where we put values computed starting from the selected one
        ccScalarField * newfield = new ccScalarField("derived field");
        newfield->resize(field->currentSize());

        for (int i = 0 ; i < field->currentSize(); ++i)
        {
            // get back the value:
            float x = field->getValue(i);

            // compute somthing new with it:
            float sinx = sin(x);

            // overwrite the field value
            newfield->setValue(i, sinx);
        }



        int id  = pc->addScalarField(newfield);
        pc->setCurrentDisplayedScalarField(id);
        pc->showSF(true);


        newfield->computeMinAndMax();


        m_app->redrawAll();
        m_app->refreshAll();
    }



}

void AdvancedDummyPlugin::doCreateCloud()
{
    //this function will create and add a new cloud to cludcompare!
    // we included some headers on top for being able to do that:
    // the cloud is created as a pointer
    ccPointCloud * cloud = new ccPointCloud("My very special point cloud");

    cloud->reserve(2); // we will add only 2 points! resize the point cloud
    // to allocate all the space we will need
    // notice we "reserved" the cloud, and not "resized"
    // when resizing the points are actually created and setted to arbitrary positions
    // when reserving the memory is allocated by the points are not added to the cloud

    // we then create a vertex:
    CCVector3 vertex1 (0,0,0);
    // add it to the point cloud:
    cloud->addPoint(vertex1);

    // we now create another verted to be added to the cloud:
    CCVector3 vertex2 (0,1,1);
    cloud->addPoint(vertex2);

    // we might want to add a scalar field representing some variables:
    ccScalarField * field = new ccScalarField("Point field");

    // resize the scalar field to the size of the cloud
    field->resize(cloud->size()) ;

    // now we populate the SF programmatically with increasing numbers:
    // obvisouly you could do the same with points if you want
    for (int i =0; i < cloud->size(); i++  )
    {
        field->setValue(i, float(i-20*2));
    }


    // now we add the field to the cloud
    int field_id = cloud->addScalarField(field);
    // we might want to set this SF as active in the window
    cloud->setCurrentDisplayedScalarField(field_id);
    cloud->showSF(true);


    //we also set the point size, so to see something
    cloud->setPointSize(10);


    // this must be called on the field to update
    // some "metadata" about min and max used in visualization/coloring etc
    field->computeMinAndMax();


    // now we can add the point cloud to the main window
    m_app->addToDB(cloud);

    // ask CloudCompare to redraw entities on screen
    m_app->redrawAll();

    // here we are!
}





//This method should return the plugin icon (it will be used mainly
//if your plugin as several actions in which case CC will create a
//dedicated sub-menu entry with this icon.
QIcon AdvancedDummyPlugin::getIcon() const
{
    //open AdvancedDummyPlugin.qrc (text file), update the "prefix" and the
    //icon(s) filename(s). Then save it with the right name (yourPlugin.qrc).
    //(eventually, remove the original AdvancedDummyPlugin.qrc file!)
    return QIcon(":/CC/plugin/AdvancedDummyPlugin/icon.png");
}

#ifndef CC_QT5
//Don't forget to replace 'AdvancedDummyPlugin' by your own plugin class name here also!
Q_EXPORT_PLUGIN2(AdvancedDummyPlugin,AdvancedDummyPlugin);
#endif
