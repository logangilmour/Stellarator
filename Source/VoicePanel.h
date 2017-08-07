//
//  VoicePanel.h
//  Stellarator
//
//  Created by Logan Gilmour on 2017-08-06.
//
//

#ifndef VoicePanel_h
#define VoicePanel_h


#include <stdio.h>

class VoicePanel : public Component
{
public:
    VoicePanel();
    void paint (Graphics&) override;
    void resized() override;
};

#endif /* VoicePanel_h */
