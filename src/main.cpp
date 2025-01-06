#include <Geode/Geode.hpp>
#include <Geode/modify/GJPathsLayer.hpp>

using namespace geode::prelude;

const int PATH_PRICE = 25000;

class $modify(ModdedGJPathsLayer, GJPathsLayer)
{
    bool init()
    {
        if (!GJPathsLayer::init())
            return false;

        //  This will definetly change when Node IDs
        //  are provided for this layer, whenever that happens
        auto GSM = GameStatsManager::sharedState();
        auto AM = AchievementManager::sharedState();
        auto activePath = GSM->m_activePath;

        //  Orb stats
        auto counterSprite = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
        counterSprite->setPosition({m_mainLayer->getContentWidth() - 22, m_mainLayer->getContentHeight() - 22});
        counterSprite->setID("orbs-icon"_spr);

        auto counterLabel = CCLabelBMFont::create(fmt::format("{}", GSM->getStat("14")).c_str(), "bigFont.fnt");
        counterLabel->setPosition({counterSprite->getPositionX() - 16, counterSprite->getPositionY()});
        counterLabel->setAnchorPoint({1, 0.5});
        counterLabel->setID("orbs-label"_spr);
        counterLabel->setScale(0.6f);

        auto counterBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
        counterBG->setPosition({counterSprite->getPositionX() + 14, counterSprite->getPositionY()});
        counterBG->setContentSize({148, 24});
        counterBG->setAnchorPoint({1, 0.5});
        counterBG->setColor({0, 0, 0});
        counterBG->setOpacity(150);
        counterBG->setZOrder(-1);

        m_mainLayer->addChild(counterBG);
        m_mainLayer->addChild(counterSprite);
        m_mainLayer->addChild(counterLabel);

        //  Cost to buy everything
        auto totalCost = 0;
        for (auto ii = 1; ii <= 10; ii++)
            totalCost += !AM->isAchievementEarned(fmt::format("geometry.ach.path{:02}.00", ii).c_str()) * PATH_PRICE;

        //  If all the paths weren't bought, adds the Price label
        if (totalCost != 0)
        {
            auto priceText = CCLabelBMFont::create("To Buy all paths:", "goldFont.fnt");
            priceText->setPosition({counterBG->getPositionX(), counterBG->getPositionY() - 28});
            priceText->setID("price-text"_spr);
            priceText->setAnchorPoint({1, 0});
            priceText->setScale(0.6f);

            auto priceLabel = CCLabelBMFont::create(fmt::format("{}", totalCost).c_str(), "bigFont.fnt");
            priceLabel->setPosition({priceText->getPositionX(), priceText->getPositionY() - 10});
            priceLabel->setColor({0, 255, 255});
            priceLabel->setAnchorPoint({1, 0.5});
            priceLabel->setID("orbs-cost"_spr);
            priceLabel->setScale(0.6f);

            m_mainLayer->addChild(priceText);
            m_mainLayer->addChild(priceLabel);
        }

        //  Doesn't add the arrow if there's no active path
        if (activePath == 0)
            return true;

        //  Gets the button Menu
        //  (This will change when Node IDs for the PathLayer exists)
        if (auto menu = static_cast<CCMenu *>(m_mainLayer->getChildren()->objectAtIndex(1)))
        {
            for (auto ii = 1; ii < menu->getChildrenCount(); ii++)
            {
                //  Gets the button's position
                auto button = static_cast<CCMenuItemSpriteExtra *>(menu->getChildren()->objectAtIndex(ii));
                CCPoint buttonPos = {164.5f + button->getPositionX(), 280.f + button->getPositionY()};

                //  If the button is the active path, creates the arrow
                if (ii == activePath - 29)
                {
                    //  Arrow sprite
                    auto arrow = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
                    arrow->setPosition({buttonPos.x, buttonPos.y + 25});
                    arrow->setID("active-path-arrow"_spr);
                    arrow->setZOrder(50);

                    //  Arrow's animation
                    auto moveUp = CCEaseInOut::create(CCMoveTo::create(1.0f, {buttonPos.x, buttonPos.y + 30}), 1.8f);
                    auto moveDown = CCEaseInOut::create(CCMoveTo::create(1.0f, {buttonPos.x, buttonPos.y + 25}), 1.8f);
                    arrow->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(moveUp, moveDown)));

                    m_mainLayer->addChild(arrow);
                };
            }
        }

        return true;
    }
};