/*
 * Copyright 2012-2013 Falltergeist Developers.
 *
 * This file is part of Falltergeist.
 *
 * Falltergeist is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Falltergeist is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Falltergeist.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../States/PlayerEditNameState.h"
#include "../Engine/Game.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/Surface.h"
#include "../UI/TextArea.h"
#include "../UI/ImageButton.h"
#include "../Engine/Player.h"
#include "ctype.h"

namespace Falltergeist
{

PlayerEditNameState::PlayerEditNameState(Game * game) : State(game)
{
}

void PlayerEditNameState::init()
{
    State::init();

    _keyCodes = new std::map<char, char>;
    _keyCodes->insert(std::make_pair(38, 'a'));
    _keyCodes->insert(std::make_pair(56, 'b'));
    _keyCodes->insert(std::make_pair(54, 'c'));
    _keyCodes->insert(std::make_pair(40, 'd'));
    _keyCodes->insert(std::make_pair(26, 'e'));
    _keyCodes->insert(std::make_pair(41, 'f'));
    _keyCodes->insert(std::make_pair(42, 'g'));
    _keyCodes->insert(std::make_pair(43, 'h'));
    _keyCodes->insert(std::make_pair(31, 'i'));
    _keyCodes->insert(std::make_pair(44, 'j'));
    _keyCodes->insert(std::make_pair(45, 'k'));
    _keyCodes->insert(std::make_pair(46, 'l'));
    _keyCodes->insert(std::make_pair(58, 'm'));
    _keyCodes->insert(std::make_pair(57, 'n'));
    _keyCodes->insert(std::make_pair(32, 'o'));
    _keyCodes->insert(std::make_pair(33, 'p'));
    _keyCodes->insert(std::make_pair(24, 'q'));
    _keyCodes->insert(std::make_pair(27, 'r'));
    _keyCodes->insert(std::make_pair(39, 's'));
    _keyCodes->insert(std::make_pair(28, 't'));
    _keyCodes->insert(std::make_pair(30, 'u'));
    _keyCodes->insert(std::make_pair(55, 'v'));
    _keyCodes->insert(std::make_pair(25, 'w'));
    _keyCodes->insert(std::make_pair(53, 'x'));
    _keyCodes->insert(std::make_pair(29, 'y'));
    _keyCodes->insert(std::make_pair(52, 'z'));
    _keyCodes->insert(std::make_pair(10, '1'));
    _keyCodes->insert(std::make_pair(11, '2'));
    _keyCodes->insert(std::make_pair(12, '3'));
    _keyCodes->insert(std::make_pair(13, '4'));
    _keyCodes->insert(std::make_pair(14, '5'));
    _keyCodes->insert(std::make_pair(15, '6'));
    _keyCodes->insert(std::make_pair(16, '7'));
    _keyCodes->insert(std::make_pair(17, '8'));
    _keyCodes->insert(std::make_pair(18, '9'));
    _keyCodes->insert(std::make_pair(19, '0'));

    _timer = SDL_GetTicks();

    Surface * bg = new Surface(_game->resourceManager()->surface("art/intrface/charwin.frm"));
    bg->setX(22);
    bg->setY(0);
    bg->setXOffset(0);
    bg->setYOffset(0);

    Surface * nameBox = new Surface(_game->resourceManager()->surface("art/intrface/namebox.frm"));
    nameBox->setX(35);
    nameBox->setY(10);
    nameBox->setXOffset(0);
    nameBox->setYOffset(0);

    Surface * doneBox = new Surface(_game->resourceManager()->surface("art/intrface/donebox.frm"));
    doneBox->setX(35);
    doneBox->setY(40);
    doneBox->setXOffset(0);
    doneBox->setYOffset(0);

    libfalltergeist::MsgFileType * msg = _game->resourceManager()->msgFileType("text/english/game/editor.msg");
    TextArea * doneLabel = new TextArea(msg->message(100), 65, 43);
    doneLabel->setColor(0xffb89c28)->setFont("font3.aaf");

    ImageButton * doneButton= new ImageButton("art/intrface/lilredup.frm", "art/intrface/lilreddn.frm", 45, 43);
    doneButton->onLeftButtonClick((EventHandler) &PlayerEditNameState::onDoneButtonClick);

    _name = new TextArea(_game->player()->name(), 43, 15);
    _name->onKeyboardRelease((EventHandler) &PlayerEditNameState::onKeyboardPress);

    _cursor = new Surface(5, 8, 83, 15);
    _cursor->fill(0xFF3FF800);

    add(bg);
    add(nameBox);
    add(doneBox);
    add(doneLabel);
    add(doneButton);
    add(_name);
    add(_cursor);
}

void PlayerEditNameState::onKeyboardPress(Event * event)
{

    std::string text(_name->text());

    if (event->keyCode() == 22) //backspace
    {
        if (text.length() > 0)
        {
            text = text.substr(0, text.length() - 1);
            _name->setText(text.c_str());
            return;
        }
        return;
    }

    if (event->keyCode() == 36) //enter
    {
        return onDoneButtonClick(event);
    }


    if (text.length() == 11) return;

    if (_keyCodes->find(event->keyCode()) != _keyCodes->end())
    {
        char chr = _keyCodes->at(event->keyCode());

        if (event->SDLEvent()->key.keysym.mod & (KMOD_CAPS | KMOD_LSHIFT))
        {
            text += toupper(chr);
        }
        else
        {
            text += chr;
        }
        _name->setText(text.c_str());
    }
}

void PlayerEditNameState::onDoneButtonClick(Event * event)
{
    std::string text(_name->text());
    if (text.length() > 0)
    {
        _game->player()->setName(text.c_str());
    }
    _game->popState();
}

PlayerEditNameState::~PlayerEditNameState()
{
    delete _keyCodes;
}

void PlayerEditNameState::think()
{
    if (!initialized) return;
    if (SDL_GetTicks() - _timer > 300)
    {
        if (_cursor->visible())
        {
            _cursor->setVisible(false);
        }
        else
        {
            _cursor->setVisible(true);
        }
        _timer = SDL_GetTicks();
    }

    _cursor->setX(_name->width() + 45);
}

}