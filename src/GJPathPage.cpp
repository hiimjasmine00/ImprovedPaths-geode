#include <Geode/Geode.hpp>
#include <Geode/modify/GJPathPage.hpp>

using namespace geode::prelude;

//  Structure for Icons
struct IconParameters : public CCObject
{
    UnlockType p_unlockType;
    int p_iconID;

    /// @brief
    /// @param icon
    /// @param id

    IconParameters(UnlockType type, int id) : p_unlockType(type), p_iconID(id)
    {
        this->autorelease();
    }
};

class $modify(ModdedGJPathPage, GJPathPage)
{
    static GJPathPage *create(int p0, GJPathsLayer *p1)
    {
        //  Adds the Treasure Room Sheet into the cache for this layer (Since I use the Chest platform)
        auto SFC = CCSpriteFrameCache::get();
        SFC->addSpriteFramesWithFile("TreasureRoomSheet.plist");
        
        //  Creates the layer and window size
        auto layer = GJPathPage::create(p0, p1);
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        //  For the Icon you get from the special chest upon completing the path
        //  Somehow, it doesn't have their own achievement
        std::pair<UnlockType, int> finalRewardIcon;
        ccColor3B color1, color2;

        switch (p0)
        {
        case 1:
            // Fire
            finalRewardIcon = {UnlockType::Swing, 19};
            color1 = {255, 75, 0};
            color2 = {255, 200, 0};
            break;

        case 2:
            // Ice
            finalRewardIcon = {UnlockType::Robot, 48};
            color1 = {0, 150, 255};
            color2 = {0, 255, 255};
            break;

        case 3:
            // Poison
            finalRewardIcon = {UnlockType::Bird, 146};
            color1 = {0, 160, 0};
            color2 = {0, 255, 0};
            break;

        case 4:
            // Shadow
            finalRewardIcon = {UnlockType::Robot, 40};
            color1 = {150, 0, 200};
            color2 = {255, 50, 255};
            break;

        case 5:
            // Lava
            finalRewardIcon = {UnlockType::Ship, 94};
            color1 = {175, 0, 0};
            color2 = {255, 50, 25};
            break;

        case 6:
            // Earth
            finalRewardIcon = {UnlockType::Ship, 91};
            color1 = {163, 98, 70};
            color2 = {253, 224, 160};
            break;

        case 7:
            // Blood
            finalRewardIcon = {UnlockType::Ship, 98};
            color1 = {150, 0, 0};
            color2 = {255, 0, 0};
            break;

        case 8:
            // Metal
            finalRewardIcon = {UnlockType::Cube, 359};
            color1 = {128, 128, 128};
            color2 = {255, 255, 255};
            break;

        case 9:
            // Light
            finalRewardIcon = {UnlockType::Robot, 66};
            color1 = {255, 250, 127};
            color2 = {255, 255, 192};
            break;

        case 10:
            // Soul
            finalRewardIcon = {UnlockType::Swing, 360};
            color1 = {100, 0, 150};
            color2 = {255, 0, 125};
            break;

        default:
            finalRewardIcon = {UnlockType::Cube, 1};
            break;
        }

        auto GM = GameManager::sharedState();

        //  Creates the extra menu for the chest reward
        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        menu->setID("mod-menu"_spr);
        layer->m_mainLayer->addChild(menu);

        CCSprite *sprite;
        if (GM->isIconUnlocked(finalRewardIcon.second, GM->unlockTypeToIconType((int)finalRewardIcon.first)))
        {
            sprite = GJItemIcon::create(finalRewardIcon.first, finalRewardIcon.second, color1, color2, false, false, false, color2);

            auto chestSprite = CCSprite::create();
            auto chestTop = CCSprite::createWithSpriteFrameName("chest_06_04_001.png");
            auto chestBottom = CCSprite::createWithSpriteFrameName("chest_06_04_back_001.png");

            chestTop->setOpacity(100);
            chestBottom->setOpacity(100);

            chestSprite->setScale(0.4);
            chestSprite->addChild(chestTop);
            chestSprite->addChild(chestBottom, -1);
            chestSprite->setPosition({winSize.width / 2, winSize.height / 2 - 100.0f});

            layer->m_mainLayer->addChild(chestSprite);
        }
        else
        {
            sprite = CCSprite::createWithSpriteFrameName("chest_06_02_001.png");
            sprite->setScale(0.4);
        }

        auto chestButton = CCMenuItemSpriteExtra::create(
            sprite,
            layer,
            menu_selector(ModdedGJPathPage::onLastRewardChest));

        chestButton->setUserObject(new IconParameters(finalRewardIcon.first, finalRewardIcon.second));
        chestButton->setPosition({winSize.width / 2, winSize.height / 2 - 100.0f});
        chestButton->setID("final-reward-chest");
        chestButton->setContentSize({50, 50});

        auto platform = CCSprite::createWithSpriteFrameName("chestPlatform_01_001.png");
        platform->setPosition({winSize.width / 2, winSize.height / 2 - 120.0f});
        platform->setColor(color1);

        sprite->setPosition({25, 25});
        menu->addChild(chestButton);
        menu->setZOrder(5);

        layer->m_mainLayer->addChild(platform);

        return layer;
    }

    void onLastRewardChest(CCObject *sender)
    {
        auto parameters = static_cast<IconParameters *>(
            static_cast<CCNode *>(sender)->getUserObject());

        ItemInfoPopup::create(parameters->p_iconID, parameters->p_unlockType)->show();
    };
};