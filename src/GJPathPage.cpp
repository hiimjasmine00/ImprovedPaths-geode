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
        auto showFinalReward = Mod::get()->getSettingValue<bool>("last-reward");

        if (!showFinalReward)
            return GJPathPage::create(p0, p1);

        //  Adds the Treasure Room Sheet into the cache for this layer (Since I use the Chest platform)
        CCSpriteFrameCache::get()->addSpriteFramesWithFile("TreasureRoomSheet.plist");

        //  Creates the layer and window size
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto layer = GJPathPage::create(p0, p1);

        //  For the Icon you get from the special chest upon completing the path
        //  Somehow, it doesn't have their own achievement
        std::pair<UnlockType, int> iconData;
        ccColor3B color1, color2;

        switch (p0)
        {
        case 1:
            // Fire
            iconData = {UnlockType::Swing, 19};
            color1 = {255, 75, 0};
            color2 = {255, 200, 0};
            break;

        case 2:
            // Ice
            iconData = {UnlockType::Robot, 48};
            color1 = {0, 150, 255};
            color2 = {0, 255, 255};
            break;

        case 3:
            // Poison
            iconData = {UnlockType::Bird, 146};
            color1 = {0, 160, 0};
            color2 = {0, 255, 0};
            break;

        case 4:
            // Shadow
            iconData = {UnlockType::Robot, 40};
            color1 = {150, 0, 200};
            color2 = {255, 50, 255};
            break;

        case 5:
            // Lava
            iconData = {UnlockType::Ship, 94};
            color1 = {175, 0, 0};
            color2 = {255, 50, 25};
            break;

        case 6:
            // Earth
            iconData = {UnlockType::Ship, 91};
            color1 = {163, 98, 70};
            color2 = {253, 224, 160};
            break;

        case 7:
            // Blood
            iconData = {UnlockType::Ship, 98};
            color1 = {150, 0, 0};
            color2 = {255, 0, 0};
            break;

        case 8:
            // Metal
            iconData = {UnlockType::Cube, 359};
            color1 = {128, 128, 128};
            color2 = {255, 255, 255};
            break;

        case 9:
            // Light
            iconData = {UnlockType::Robot, 66};
            color1 = {255, 250, 127};
            color2 = {255, 255, 192};
            break;

        case 10:
            // Soul
            iconData = {UnlockType::Swing, 36};
            color1 = {100, 0, 150};
            color2 = {255, 0, 125};
            break;

        default:
            iconData = {UnlockType::Cube, 1};
            break;
        }

        auto GM = GameManager::sharedState();

        //  Creates the extra menu for the chest reward
        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        menu->setID("mod-menu"_spr);
        layer->m_mainLayer->addChild(menu);

        //  Creates the Sprite of the last reward icon
        auto icon = GJItemIcon::create(iconData.first, iconData.second, color1, color2, false, false, false, color2);
        if (!GM->isIconUnlocked(iconData.second, GM->unlockTypeToIconType((int)iconData.first)))
            icon->changeToLockedState(1);

        //  Adds the platform for aesthetic
        auto platform = CCSprite::createWithSpriteFrameName("chestPlatform_01_001.png");
        platform->setPosition({winSize.width / 2, winSize.height / 2 - 125.0f});
        platform->setID("final-reward-platform"_spr);
        platform->setColor(color1);
        layer->m_mainLayer->addChild(platform);

        //  Label for the icon
        auto label = CCLabelBMFont::create("Final Reward:", "goldFont.fnt");
        label->setPosition({winSize.width / 2, winSize.height / 2 - 80.0f});
        label->setID("final-reward-label"_spr);
        label->setScale(0.5f);
        layer->m_mainLayer->addChild(label);

        //  Button for the Icon
        auto rewardButton = CCMenuItemSpriteExtra::create(
            icon,
            layer,
            menu_selector(ModdedGJPathPage::onLastRewardChest));
        rewardButton->setUserObject(new IconParameters(iconData.first, iconData.second));
        rewardButton->setPosition({winSize.width / 2, winSize.height / 2 - 100.0f});
        rewardButton->setID("final-reward"_spr);

        rewardButton->setContentSize({50, 50});
        icon->setPosition({25, 25});

        menu->addChild(rewardButton);
        menu->setZOrder(5);
        return layer;
    }

    void onLastRewardChest(CCObject *sender)
    {
        auto parameters = static_cast<IconParameters *>(
            static_cast<CCNode *>(sender)->getUserObject());

        ItemInfoPopup::create(parameters->p_iconID, parameters->p_unlockType)->show();
    };
};