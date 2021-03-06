#include "lxgui/impl/gui_sfml_material.hpp"
#include "lxgui/impl/gui_sfml_renderer.hpp"
#include <lxgui/gui_out.hpp>
#include <lxgui/gui_exception.hpp>
#include <lxgui/utils_string.hpp>

namespace lxgui {
namespace gui {
namespace sfml
{
material::material(uint uiWidth, uint uiHeight, bool bRenderTarget, wrap mWrap, filter mFilter) :
    uiWidth_(uiWidth), uiHeight_(uiHeight), uiRealWidth_(uiWidth), uiRealHeight_(uiHeight),
    mWrap_(mWrap), mFilter_(mFilter)
{
    if (uiRealWidth_ > sf::Texture::getMaximumSize() ||
        uiRealHeight_ > sf::Texture::getMaximumSize())
    {
        throw gui::exception("gui::sfml::material", "Texture dimensions not supported by graphics card : ("+
            utils::to_string(uiRealWidth_)+" x "+
            utils::to_string(uiRealHeight_)+")."
        );
    }

    bRenderTarget_ = bRenderTarget;

    if (bRenderTarget_)
    {
        if (!mRenderTexture_.create(uiRealWidth_, uiRealHeight_))
        {
            throw gui::exception("gui::sfml::material", "Could not create render target with dimensions "+
                utils::to_string(uiWidth)+" x "+utils::to_string(uiHeight)+".");
        }
        mRenderTexture_.setSmooth(mFilter == filter::LINEAR);
        mRenderTexture_.setRepeated(mWrap == wrap::REPEAT);
    }
    else
    {
        if (!mTexture_.create(uiRealWidth_, uiRealHeight_))
        {
            throw gui::exception("gui::sfml::material", "Could not create texture with dimensions "+
                utils::to_string(uiWidth)+" x "+utils::to_string(uiHeight)+".");
        }
        mTexture_.setSmooth(mFilter == filter::LINEAR);
        mTexture_.setRepeated(mWrap == wrap::REPEAT);
    }
}

material::material(const sf::Image& mData, wrap mWrap, filter mFilter)
{
    bRenderTarget_ = false;
    mTexture_.loadFromImage(mData);
    mTexture_.setSmooth(mFilter == filter::LINEAR);
    mTexture_.setRepeated(mWrap == wrap::REPEAT);

    const uint uiWidth  = mTexture_.getSize().x;
    const uint uiHeight = mTexture_.getSize().y;
    uiWidth_ = uiWidth;
    uiHeight_ = uiHeight;
    mWrap_ = mWrap;
    mFilter_ = mFilter;
    uiRealWidth_ = uiWidth;
    uiRealHeight_ = uiHeight;
}

material::material(const std::string& sFileName, wrap mWrap, filter mFilter)
{
    bRenderTarget_ = false;
    sf::Image mData;
    if (!mData.loadFromFile(sFileName))
        throw utils::exception("gui::sfml::material", "loading failed: '"+sFileName+"'.");
    premultiply_alpha(mData);
    mTexture_.loadFromImage(mData);
    mTexture_.setSmooth(mFilter == filter::LINEAR);
    mTexture_.setRepeated(mWrap == wrap::REPEAT);

    const uint uiWidth  = mTexture_.getSize().x;
    const uint uiHeight = mTexture_.getSize().y;
    uiWidth_ = uiWidth;
    uiHeight_ = uiHeight;
    mWrap_ = mWrap;
    mFilter_ = mFilter;
    uiRealWidth_ = uiWidth;
    uiRealHeight_ = uiHeight;
}

void material::set_wrap(wrap mWrap)
{
    mWrap_ = mWrap;

    if (bRenderTarget_)
        mRenderTexture_.setRepeated(mWrap == wrap::REPEAT);
    else
        mTexture_.setRepeated(mWrap == wrap::REPEAT);
}

void material::set_filter(filter mFilter)
{
    mFilter_ = mFilter;

    if (bRenderTarget_)
        mRenderTexture_.setSmooth(mFilter == filter::LINEAR);
    else
        mTexture_.setSmooth(mFilter == filter::LINEAR);
}

void material::premultiply_alpha(sf::Image& mData)
{
    const uint uiWidth = mData.getSize().x;
    const uint uiHeight = mData.getSize().y;
    for (uint x = 0; x < uiWidth; ++x)
    for (uint y = 0; y < uiHeight; ++y)
    {
        sf::Color c = mData.getPixel(x, y);
        float a = c.a/255.0f;
        c.r *= a;
        c.g *= a;
        c.b *= a;
        mData.setPixel(x, y, c);
    }
}

float material::get_width() const
{
    return uiWidth_;
}

float material::get_height() const
{
    return uiHeight_;
}

float material::get_real_width() const
{
    return uiRealWidth_;
}

float material::get_real_height() const
{
    return uiRealHeight_;
}

bool material::set_dimensions(uint uiWidth, uint uiHeight)
{
    if (!bRenderTarget_) return false;

    if (uiWidth > sf::Texture::getMaximumSize() || uiHeight > sf::Texture::getMaximumSize())
        return false;

    if (uiWidth > uiRealWidth_ || uiHeight > uiRealHeight_)
    {
        uiWidth_      = uiWidth;
        uiHeight_     = uiHeight;
        // SFML is not efficient at resizing render texture, so use an exponential growth pattern
        // to avoid re-allocating a new render texture on every resize operation.
        if (uiWidth > uiRealWidth_)
            uiRealWidth_  = uiWidth + uiWidth/2;
        if (uiHeight > uiRealHeight_)
            uiRealHeight_ = uiHeight + uiHeight/2;

        if (!mRenderTexture_.create(uiRealWidth_, uiRealHeight_))
        {
            throw gui::exception("gui::sfml::material", "Could not create render target with dimensions "+
                utils::to_string(uiRealWidth_)+" x "+utils::to_string(uiRealHeight_)+".");
        }

        mRenderTexture_.setSmooth(mFilter_ == filter::LINEAR);
        mRenderTexture_.setRepeated(mWrap_ == wrap::REPEAT);

        return true;
    }
    else
    {
        uiWidth_  = uiWidth;
        uiHeight_ = uiHeight;
        return false;
    }
}

sf::RenderTexture* material::get_render_texture()
{
    if (!bRenderTarget_) return nullptr;

    return &mRenderTexture_;
}

const sf::Texture* material::get_texture() const
{
    if (bRenderTarget_)
        return &mRenderTexture_.getTexture();
    else
        return &mTexture_;
}
}
}
}
