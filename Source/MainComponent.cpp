/*
    MainComponent.cpp
    Copyright (C) 2017 Jonathon Racz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MainComponent.h"

MainContentComponent::MainContentComponent()
{
    editor.setTextToShowWhenEmpty("Input hex...", Colours::grey);
    output.setTextWhenNothingSelected("Select MIDI output...");

    setSize(280, 104);

    addAndMakeVisible(editor);
    addAndMakeVisible(output);
    addAndMakeVisible(send);
    addAndMakeVisible(rescan);

    send.addListener(this);
    rescan.addListener(this);

    send.setButtonText("Send MIDI");
    rescan.setButtonText("Rescan");

    output.clear();
    output.addItemList(MidiOutput::getDevices(), 1);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint(Graphics& g)
{
}

void MainContentComponent::resized()
{
    editor.setBounds(8, 40, 264, 24);
    output.setBounds(8, 8, 184, 24);
    send.setBounds(96, 72, 102, 24);
    rescan.setBounds(200, 8, 72, 24);
}

void MainContentComponent::buttonClicked(Button* button)
{
    if (button == &send)
    {
        if (output.getSelectedId() != 0)
        {
            auto dest = std::unique_ptr<MidiOutput>(
                MidiOutput::openDevice(
                    MidiOutput::getDevices().indexOf(
                        output.getText())));

            MemoryBlock midiData = editor.convertToData();

            dest->sendMessageNow(MidiMessage(midiData.getData(), midiData.getSize()));
        }
    }
    else if (button == &rescan)
    {
        output.clear();
        output.addItemList(MidiOutput::getDevices(), 1);
        output.repaint();
    }
}
