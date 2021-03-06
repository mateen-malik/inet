//
// Copyright (C) 2020 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include "inet/visualizer/osg/scene/NetworkNodeOsgVisualizer.h"

#include <omnetpp/osgutil.h>

#include "inet/common/ModuleAccess.h"
#include "inet/visualizer/osg/util/OsgScene.h"
#include "inet/visualizer/osg/util/OsgUtils.h"

namespace inet {

namespace visualizer {

Define_Module(NetworkNodeOsgVisualizer);

NetworkNodeOsgVisualizer::~NetworkNodeOsgVisualizer()
{
    for (auto& it : networkNodeVisualizations) {
        auto networkNodeVisualization = it.second;
        while (networkNodeVisualization->getNumAnnotations() != 0)
            networkNodeVisualization->removeAnnotation(0);
    }
}

void NetworkNodeOsgVisualizer::initialize(int stage)
{
    NetworkNodeVisualizerBase::initialize(stage);
    if (!hasGUI()) return;
    if (stage == INITSTAGE_LOCAL) {
        displayModuleName = par("displayModuleName");
        for (cModule::SubmoduleIterator it(visualizationSubjectModule); !it.end(); it++) {
            auto networkNode = *it;
            if (isNetworkNode(networkNode) && nodeFilter.matches(networkNode)) {
                auto visualization = createNetworkNodeVisualization(networkNode);
                addNetworkNodeVisualization(visualization);
            }
        }
    }
}

void NetworkNodeOsgVisualizer::refreshDisplay() const
{
    for (auto it : networkNodeVisualizations) {
        auto networkNode = it.first;
        auto visualization = it.second;
        auto position = getPosition(networkNode);
        auto orientation = getOrientation(networkNode);
        visualization->setPosition(osg::Vec3d(position.x, position.y, position.z));
        visualization->setAttitude(osg::Quat(osg::Vec4d(orientation.v.x, orientation.v.y, orientation.v.z, orientation.s)));
    }
}

NetworkNodeOsgVisualization *NetworkNodeOsgVisualizer::createNetworkNodeVisualization(cModule *networkNode) const
{
    return new NetworkNodeOsgVisualization(networkNode, displayModuleName);
}

NetworkNodeOsgVisualization *NetworkNodeOsgVisualizer::getNetworkNodeVisualization(const cModule *networkNode) const
{
    auto it = networkNodeVisualizations.find(networkNode);
    return it == networkNodeVisualizations.end() ? nullptr : it->second;
}

void NetworkNodeOsgVisualizer::addNetworkNodeVisualization(NetworkNodeVisualization *networkNodeVisualization)
{
    auto networkNodeOsgVisualization = check_and_cast<NetworkNodeOsgVisualization *>(networkNodeVisualization);
    networkNodeVisualizations[networkNodeOsgVisualization->networkNode] = networkNodeOsgVisualization;
    auto scene = inet::osg::TopLevelScene::getSimulationScene(visualizationTargetModule);
    scene->addChild(networkNodeOsgVisualization);
}

void NetworkNodeOsgVisualizer::removeNetworkNodeVisualization(NetworkNodeVisualization *networkNodeVisualization)
{
    auto networkNodeOsgVisualization = check_and_cast<NetworkNodeOsgVisualization *>(networkNodeVisualization);
    networkNodeVisualizations.erase(networkNodeVisualizations.find(networkNodeOsgVisualization->networkNode));
    auto scene = inet::osg::TopLevelScene::getSimulationScene(visualizationTargetModule);
    scene->removeChild(networkNodeOsgVisualization);
}

} // namespace visualizer

} // namespace inet

