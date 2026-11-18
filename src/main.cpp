#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

class MenuPopup : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("My Menu");
        auto t = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(MenuPopup::onNoclip), 0.8f);
        t->toggle(Mod::get()->getSavedValue<bool>("noclip", false));
        m_buttonMenu->addChildAtPosition(t, Anchor::Center, {-60, 0});
        auto l = CCLabelBMFont::create("Noclip", "bigFont.fnt");
        l->setScale(0.6f);
        m_mainLayer->addChildAtPosition(l, Anchor::Center, {20, 0});
        return true;
    }
    void onNoclip(CCObject* s) {
        auto t = static_cast<CCMenuItemToggler*>(s);
        Mod::get()->setSavedValue("noclip", !t->isToggled());
    }
public:
    static MenuPopup* create() {
        auto r = new MenuPopup();
        if (r->initAnchored(240.f, 160.f)) { r->autorelease(); return r; }
        delete r; return nullptr;
    }
};

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
            this, menu_selector(MyMenuLayer::onMyMenu));
        auto menu = this->getChildByID("bottom-menu");
        menu->addChild(btn);
        menu->updateLayout();
        return true;
    }
    void onMyMenu(CCObject*) { MenuPopup::create()->show(); }
};

class $modify(PlayLayer) {
    void destroyPlayer(PlayerObject* p, GameObject* g) {
        if (Mod::get()->getSavedValue<bool>("noclip", false)) return;
        PlayLayer::destroyPlayer(p, g);
    }
};
