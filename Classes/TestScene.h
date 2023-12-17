/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once

#include "cocos2d.h"

namespace cocos2d {
namespace ui {
    class Button;
}
}

class TestScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(TestScene);

private:
    /**
     * Character that jumps after the user presses the button.
     */
    cocos2d::Sprite* character = nullptr;

    /**
     * Node used to implement the parallax effect whenever the user slides
     * a finger across the screen.
     */
    cocos2d::ParallaxNode* parallax = nullptr;

    /**
     * Contains the maximum distance by which a parallax node can be shifted
     * horizontally and vertically.
     */
    cocos2d::Size parallaxConstraints = {};

    /**
     * Current cursor position on the screen. Combined with prevCursorPosition
     * allows to calculate a distance the cursor travelled between
     * onCursorMove() calls.
     */
    cocos2d::Vec2 currCursorPosition = {};

    /**
     * Cursor position on the screen at the time of the last onCursorMove()
     * call.
     */
    cocos2d::Vec2 prevCursorPosition = {};

    /**
     * True between the calls of onCursorDown() and onCursorUp().
     */
    bool isCursorDown = false;

    /**
     * True while character is in the air and has not yet landed.
     */
    bool isCharacterAirborne = false;

    /**
     * Creates and populates a background layer.
     *
     * @return a background layer
     */
    cocos2d::Node* createBackground();

    /**
     * Creates and populates a middle ground layer. Arranges child nodes based
     * on the viewport dimensions.
     *
     * @param width  viewport width
     * @param height viewport height
     * @return a middle ground layer
     */
    cocos2d::Node* createMiddleLayer(float width, float height);

    /**
     * Creates and populates a foreground layer. Arranges child nodes based
     * on the viewport dimensions.
     *
     * @param width  viewport width
     * @param height viewport height
     * @return a middle ground layer
     */
    cocos2d::Node* createForeground(float width, float height);

    /**
     * Called whenever the application receives `onMouseDown` or `onTouchBegan`
     * input events.
     *
     * @param position cursor position on the screen
     */
    void onCursorDown(cocos2d::Vec2 position);

    /**
     * Called whenever the application receives `onMouseMove` or `onTouchMoved`
     * input events.
     *
     * @param position cursor position on the screen
     */
    void onCursorMove(cocos2d::Vec2 position);

    /**
     * Called whenever the application receives `onMouseUp` or `onTouchEnded`
     * input events.
     */
    void onCursorUp();

    /**
     * Called whenever the user presses the jump button.
     *
     * @param button button pressed
     */
    void onJumpButtonClick(cocos2d::ui::Button* button);
};
