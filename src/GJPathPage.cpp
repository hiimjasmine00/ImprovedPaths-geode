#include <Geode/Geode.hpp>
#include <Geode/modify/GJPathPage.hpp>

using namespace geode::prelude;

class $modify(ModdedGJPathPage, GJPathPage)
{
    struct Fields {
        ~Fields() {
            //  Removes the Treasure Room Sheet from the cache
            CCSpriteFrameCache::get()->removeSpriteFramesFromFile("TreasureRoomSheet.plist");
            CCTextureCache::get()->removeTextureForKey("TreasureRoomSheet.png");
        }
    };

    bool init(int p0, GJPathsLayer *p1)
    {
        if (!GJPathPage::init(p0, p1))
            return false;

        auto showFinalReward = Mod::get()->getSettingValue<bool>("last-reward");

        if (!showFinalReward)
            return true;

        m_fields.self();

        //  Adds the Treasure Room Sheet into the cache for this layer (Since I use the Chest platform)
        auto texture = CCTextureCache::get()->addImage("TreasureRoomSheet.png", false);
        CCSpriteFrameCache::get()->addSpriteFramesWithFile("TreasureRoomSheet.plist", texture);

        //  Creates the layer and window size
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        //  For the Icon you get from the special chest upon completing the path
        //  Somehow, it doesn't have their own achievement
        std::pair<UnlockType, int> iconData = {UnlockType::Cube, 1};
        auto GSM = GameStatsManager::sharedState();
        auto pathReward = static_cast<GJRewardItem*>(GSM->m_allSpecialChests->objectForKey(fmt::format("pr_{}", p0)));
        for (auto rewardObject : CCArrayExt<GJRewardObject*>(pathReward->m_rewardObjects))
        {
            if (rewardObject->m_specialRewardItem == SpecialRewardItem::CustomItem)
            {
                iconData = {rewardObject->m_unlockType, rewardObject->m_itemID};
                break;
            }
        }

        ccColor3B color1, color2;

        switch (p0)
        {
        case 1:
            // Fire
            color1 = {255, 75, 0};
            color2 = {255, 200, 0};
            break;

        case 2:
            // Ice
            color1 = {0, 150, 255};
            color2 = {0, 255, 255};
            break;

        case 3:
            // Poison
            color1 = {0, 160, 0};
            color2 = {0, 255, 0};
            break;

        case 4:
            // Shadow
            color1 = {150, 0, 200};
            color2 = {255, 50, 255};
            break;

        case 5:
            // Lava
            color1 = {175, 0, 0};
            color2 = {255, 50, 25};
            break;

        case 6:
            // Earth
            color1 = {163, 98, 70};
            color2 = {253, 224, 160};
            break;

        case 7:
            // Blood
            color1 = {150, 0, 0};
            color2 = {255, 0, 0};
            break;

        case 8:
            // Metal
            color1 = {128, 128, 128};
            color2 = {255, 255, 255};
            break;

        case 9:
            // Light
            color1 = {255, 250, 127};
            color2 = {255, 255, 192};
            break;

        case 10:
            // Soul
            color1 = {100, 0, 150};
            color2 = {255, 0, 125};
            break;
        }

        auto GM = GameManager::sharedState();

        //  Creates the extra menu for the chest reward
        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        menu->setID("mod-menu"_spr);
        m_mainLayer->addChild(menu);

        //  Creates the Sprite of the last reward icon
        auto icon = GJItemIcon::create(iconData.first, iconData.second, color1, color2, false, false, false, color2);
        if (!GM->isIconUnlocked(iconData.second, GM->unlockTypeToIconType((int)iconData.first)))
            icon->changeToLockedState(1);

        //  Adds the platform for aesthetic
        auto platform = CCSprite::createWithSpriteFrameName("chestPlatform_01_001.png");
        platform->setPosition({winSize.width / 2, winSize.height / 2 - 125.0f});
        platform->setID("final-reward-platform"_spr);
        platform->setColor(color1);
        m_mainLayer->addChild(platform);

        //  Label for the icon
        auto label = CCLabelBMFont::create("Final Reward:", "goldFont.fnt");
        label->setPosition({winSize.width / 2, winSize.height / 2 - 80.0f});
        label->setID("final-reward-label"_spr);
        label->setScale(0.5f);
        m_mainLayer->addChild(label);

        //  Button for the Icon
        auto rewardButton = CCMenuItemSpriteExtra::create(
            icon,
            this,
            menu_selector(ModdedGJPathPage::onLastRewardChest));
        rewardButton->setPosition({winSize.width / 2, winSize.height / 2 - 100.0f});
        rewardButton->setID("final-reward"_spr);

        rewardButton->setContentSize({50, 50});
        icon->setPosition({25, 25});

        menu->addChild(rewardButton);
        menu->setZOrder(5);

        return true;
    }

    void onLastRewardChest(CCObject *sender)
    {
        auto icon = static_cast<GJItemIcon *>(
            static_cast<CCMenuItemSpriteExtra *>(sender)->getNormalImage());

        ItemInfoPopup::create(icon->m_unlockID, icon->m_unlockType)->show();
    };
};