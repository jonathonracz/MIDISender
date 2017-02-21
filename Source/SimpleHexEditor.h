/*
    SimpleHexEditor.h
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

#pragma once

#include <JuceHeader.h>

namespace ntwelve
{

/** A *very* rough, simple, and unoptimized class which makes a TextEditor act
    as a hex editor by filtering characters and formatting the text into byte
    groups.
*/
class SimpleHexEditor :
    public TextEditor,
    public TextEditor::InputFilter,
    public TextEditor::Listener
{
public:
    SimpleHexEditor() :
        bytesPerGroup(8),
        groupsPerLine(2)
    {
        this->addListener(this);
        this->setInputFilter(this, false);
        this->setFont(Font(Font::getDefaultMonospacedFontName(), Font::getDefaultStyle(), 14.0f));
        this->setMultiLine(true);

    }

    ~SimpleHexEditor() {}

    void setBytesPerGroup(int _bytesPerGroup) { bytesPerGroup = _bytesPerGroup; }
    int getBytesPerGroup() const { return bytesPerGroup; }
    void setGroupsPerLine(int _groupsPerLine) { groupsPerLine = _groupsPerLine; }
    int getGroupsPerLine() const { return groupsPerLine; }

    void importFromData(const MemoryBlock& data)
    {
        this->setText(String::toHexString(data.getData(), static_cast<int>(data.getSize()), 0));
    }

    MemoryBlock convertToData() const
    {
        MemoryBlock ret;
        ret.loadFromHexString(this->getText());
        return ret;
    }

    void textEditorTextChanged(TextEditor& editor) override
    {
        String existingText = editor.getText();
        int oldSize = existingText.length();
        existingText = existingText.removeCharacters(" \n\t");

        String formatted;
        formatted.preallocateBytes(static_cast<size_t>(existingText.length() * 1.5f));
        for (int i = 0; i < existingText.length(); i += 2)
        {
            if (i > 0)
            {
                if (i / 2 % bytesPerGroup == 0)
                    if ((i / 2) / bytesPerGroup % groupsPerLine == 0)
                        formatted.append("\n", 2);
                    else
                        formatted.append("\t", 2);
                else
                    formatted.append(" ", 1);
            }

            formatted.append(existingText.substring(i, i + 2), 2);
        }

        editor.setText(formatted, false);

        if (editor.getCaretPosition() % 3 == 0 || oldSize < formatted.length())
        {
            editor.setCaretPosition(getCaretPosition() + formatted.length() - oldSize);
        }
    }

    String filterNewText(TextEditor& editor, const String& newInput) override
    {
        return newInput.toUpperCase().retainCharacters("0123456789ABCDEF");
    }

private:
    int bytesPerGroup;
    int groupsPerLine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleHexEditor)

};

}
