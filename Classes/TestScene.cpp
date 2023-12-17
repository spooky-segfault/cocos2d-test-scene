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

#include "TestScene.h"
#include "ui/UIButton.h"

USING_NS_CC;

/**
 * Creates an action simulating a jump.
 *
 * @param callback function called after action completion
 * @return an action
 */
FiniteTimeAction* createJumpAction(const std::function<void()>& callback);

/**
 * Creates an action simulating a jump upwards.
 *
 * @return an action
 */
FiniteTimeAction* createJumpUpAction();

/**
 * Creates an action simulating landing after a jump.
 *
 * Note: this action could be implemented using createJumpUpAction()->reverse(),
 * but unfortunately reverse() does not work correctly with animations
 * and is not implemented for ScaleTo (ScaleBy is fine).
 *
 * @return an action
 */
FiniteTimeAction* createJumpDownAction();

Scene* TestScene::createScene()
{
    return TestScene::create();
}

Node* TestScene::createBackground()
{
    return Sprite::create("background.png");
}

Node* TestScene::createMiddleLayer(float width, float height)
{
    auto layer = Node::create();

    character = Sprite::create("character_idle.png");
    character->setAnchorPoint({ 0.5f, 0.0f });
    character->setPosition({ width * 0.5f, height * 0.5f });
    layer->addChild(character);

    auto platform = Sprite::create("platform.png");
    platform->setAnchorPoint({ 0.5f, 1.0f });
    platform->setPosition({ width * 0.5f, height * 0.5f });
    layer->addChild(platform);

    return layer;
}

Node* TestScene::createForeground(float width, float height)
{
    auto layer = Node::create();

    auto cloud1 = Sprite::create("cloud_01.png");
    cloud1->setPosition({ width * -1.0f, height * 0.1f });
    layer->addChild(cloud1);

    auto cloud2 = Sprite::create("cloud_02.png");
    cloud2->setPosition({ width * 0.0f, height * 0.3f });
    layer->addChild(cloud2);

    auto cloud3 = Sprite::create("cloud_03.png");
    cloud3->setPosition({ width * 1.0f, height * 0.1f });
    layer->addChild(cloud3);

    auto cloud4 = Sprite::create("cloud_01.png");
    cloud4->setPosition({ width * 2.0f, height * 0.3f });
    layer->addChild(cloud4);

    return layer;
}

void TestScene::onCursorDown(Vec2 position)
{
    currCursorPosition = position;
    isCursorDown = true;
}

void TestScene::onCursorMove(Vec2 position)
{
    if (!isCursorDown) {
        return;
    }

    prevCursorPosition = currCursorPosition;
    currCursorPosition = position;

    Vec2 delta = currCursorPosition - prevCursorPosition;
    delta.y = 0;
    Vec2 newPosition = parallax->getPosition() + delta;
    newPosition.x = clampf(newPosition.x, -parallaxConstraints.width, parallaxConstraints.width);
    parallax->setPosition(newPosition);
}

void TestScene::onCursorUp()
{
    isCursorDown = false;
}

bool TestScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto director = Director::getInstance();
    parallax = ParallaxNode::create();
    parallax->setPosition(director->getVisibleOrigin());
    addChild(parallax);

    Size viewport = director->getVisibleSize();
    auto background = createBackground();
    parallax->addChild(background, -1, { 0.2f, 0.2f }, { viewport.width * 0.5f, viewport.height * 0.5f });
    auto middleLayer = createMiddleLayer(viewport.width, viewport.height);
    parallax->addChild(middleLayer, 0, { 1.0f, 1.0f }, {});
    auto foreground = createForeground(viewport.width, viewport.height);
    parallax->addChild(foreground, 1, { 3.0f, 3.0f }, {});
    parallaxConstraints.setSize(viewport.width * 0.5f, 0.0f);

    auto mouseEventListener = EventListenerMouse::create();
    mouseEventListener->onMouseDown = [this](Event* e) { onCursorDown(static_cast<EventMouse*>(e)->getLocation()); };
    mouseEventListener->onMouseMove = [this](Event* e) { onCursorMove(static_cast<EventMouse*>(e)->getLocation()); };
    mouseEventListener->onMouseUp = [this](Event* e) { onCursorUp(); };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseEventListener, this);

    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = [this](Touch* t, Event* e) { onCursorDown(t->getLocation()); return true; };
    touchEventListener->onTouchMoved = [this](Touch* t, Event* e) { onCursorMove(t->getLocation()); };
    touchEventListener->onTouchEnded = [this](Touch* t, Event* e) { onCursorUp(); };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);

    auto button = ui::Button::create("button_jump.png", "", "button_jump_disable.png");
    button->setPosition({ viewport.width * 0.5f, viewport.height * 0.15f });
    button->setScale(0.75f);
    button->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType t) {
        if (t == ui::Widget::TouchEventType::ENDED) {
            onJumpButtonClick(static_cast<ui::Button*>(sender));
        }
    });
    addChild(button);

    return true;
}

void TestScene::onJumpButtonClick(ui::Button* button)
{
    if (isCharacterAirborne) {
        return;
    }

    isCharacterAirborne = true;
    button->setEnabled(false);

    character->runAction(createJumpAction([=]() {
        isCharacterAirborne = false;
        button->setEnabled(true);
    }));
}

FiniteTimeAction* createJumpAction(const std::function<void()>& callback)
{
    return Sequence::create(
        createJumpUpAction(),
        createJumpDownAction(),
        CallFunc::create(callback),
        nullptr);
}

FiniteTimeAction* createJumpUpAction()
{
    Animation* idle2Cheer;
    {
        auto frames = Vector<SpriteFrame*>(2);
        frames.pushBack(SpriteFrame::create("character_idle.png", Rect(0, 0, 192, 256)));
        frames.pushBack(SpriteFrame::create("character_cheer.png", Rect(0, 0, 192, 256)));
        idle2Cheer = Animation::createWithSpriteFrames(frames, 0.05f);
    }

    return Sequence::create(
        ScaleTo::create(0.1f, 1.1f, 0.9f),
        ScaleTo::create(0.1f, 0.9f, 1.1f),
        Spawn::create(
            EaseExponentialOut::create(MoveBy::create(0.4f, Vec2(0, 100))),
            ScaleTo::create(0.1f, 1.0f, 1.0f),
            Animate::create(idle2Cheer),
            nullptr),
        nullptr);
}

FiniteTimeAction* createJumpDownAction()
{
    Animation* cheer2Idle;
    {
        auto frames = Vector<SpriteFrame*>(2);
        frames.pushBack(SpriteFrame::create("character_cheer.png", Rect(0, 0, 192, 256)));
        frames.pushBack(SpriteFrame::create("character_idle.png", Rect(0, 0, 192, 256)));
        cheer2Idle = Animation::createWithSpriteFrames(frames, 0.3f);
    }

    return Sequence::create(
        Spawn::create(
            EaseExponentialIn::create(MoveBy::create(0.4f, Vec2(0, -100))),
            ScaleTo::create(0.3f, 0.9f, 1.1f),
            Animate::create(cheer2Idle),
            Sequence::create(
                DelayTime::create(0.4f),
                ScaleTo::create(0.1f, 1.1f, 0.9f),
                nullptr),
            nullptr),
        ScaleTo::create(0.1f, 1.0f, 1.0f),
        nullptr);
}
