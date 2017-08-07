//
//  VoicePanel.cpp
//  Stellarator
//
//  Created by Logan Gilmour on 2017-08-06.
//
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "VoicePanel.h"

VoicePanel::VoicePanel(){
    setSize(200,200);
}

void VoicePanel::paint(Graphics& g){
    g.fillAll(Colours::aqua);
}

void VoicePanel::resized(){
    
}
