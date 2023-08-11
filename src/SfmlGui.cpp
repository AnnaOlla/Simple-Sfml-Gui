#include "SfmlGui.hpp"
#include <iostream>

namespace SfGui
{

DecorationSettings::DecorationSettings(const sf::Color fillColor,
                                       const sf::Color textColor,
                                       const sf::Uint32 textStyle,
                                       const float outlineThickness,
                                       const sf::Color outlineColor,
                                       const sf::Texture* backgroundTexture) :
    m_fillColor(fillColor),
    m_textColor(textColor),
    m_textStyle(textStyle),
    m_outlineThickness(outlineThickness > 0 ? -outlineThickness : outlineThickness),
    m_outlineColor(outlineColor),
    m_backgroundTexture(backgroundTexture)
{
    //ctor
}

DecorationSettings::~DecorationSettings()
{
    //dtor
}

sf::Color DecorationSettings::getFillColor() const
{
    return m_fillColor;
}

sf::Color DecorationSettings::getTextColor() const
{
    return m_textColor;
}

sf::Uint32 DecorationSettings::getTextStyle() const
{
    return m_textStyle;
}

sf::Color DecorationSettings::getOutlineColor() const
{
    return m_outlineColor;
}

float DecorationSettings::getOutlineThickness() const
{
    return m_outlineThickness;
}

const sf::Texture* DecorationSettings::getBackgroundTexture() const
{
    return m_backgroundTexture;
}

void DecorationSettings::setFillColor(const sf::Color fillColor)
{
    m_fillColor = fillColor;
}

void DecorationSettings::setTextColor(const sf::Color textColor)
{
    m_textColor = textColor;
}

void DecorationSettings::setTextStyle(const sf::Uint32 textStyle)
{
    m_textStyle = textStyle;
}

void DecorationSettings::setOutlineThickness(const float outlineThickness)
{
    m_outlineThickness = outlineThickness > 0 ? -outlineThickness : outlineThickness;
}

void DecorationSettings::setOutlineColor(const sf::Color outlineColor)
{
    m_outlineColor = outlineColor;
}

void DecorationSettings::setBackgroundTexture(const sf::Texture* backgroundTexture)
{
    m_backgroundTexture = backgroundTexture;
}

TextSettings::TextSettings(const sf::Font& font,
                           const unsigned int characterSize,
                           const TextHorizontalAlignment horizontalAlignment,
                           const TextVerticalAlignment verticalAlignment) :
    m_font(&font),
    m_characterSize(characterSize),
    m_horizontalAlignment(horizontalAlignment),
    m_verticalAlignment(verticalAlignment),
    m_fontMetrics(calculateFontMetrics())
{
    //ctor
}

TextSettings::~TextSettings()
{
    //dtor
}

const sf::Font& TextSettings::getFont() const
{
    return *m_font;
}

unsigned int TextSettings::getCharacterSize() const
{
    return m_characterSize;
}

TextHorizontalAlignment TextSettings::getHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

TextVerticalAlignment TextSettings::getVerticalAlignment() const
{
    return m_verticalAlignment;
}

const FontMetrics& TextSettings::getFontMetrics() const
{
    return m_fontMetrics;
}

void TextSettings::setFont(const sf::Font& font)
{
    m_font = &font;
    m_fontMetrics = calculateFontMetrics();
}

void TextSettings::setCharacterSize(const unsigned int characterSize)
{
    m_characterSize = characterSize;
    m_fontMetrics = calculateFontMetrics();
}

void TextSettings::setHorizontalAlignment(const TextHorizontalAlignment horizontalAlignment)
{
    m_horizontalAlignment = horizontalAlignment;
}

void TextSettings::setVerticalAlignment(const TextVerticalAlignment verticalAlignment)
{
    m_verticalAlignment = verticalAlignment;
}

FontMetrics TextSettings::calculateFontMetrics()
{
    FontMetrics metrics;

    sf::Text temp;
    temp.setFont(*m_font);
    temp.setCharacterSize(m_characterSize);

    // Just some magic to apply vertical alignment correctly
    // (some fonts will have it a little incorrect)

    // We calculate the highest line and the line which all letters stand on.
    // We use "A" as the standard
    temp.setString(L"A");
    metrics.ascenderLine = temp.getLocalBounds().top;
    metrics.baseLine = temp.getLocalBounds().height;

    // We calculate the lowest line of letters and, at last, maximum possible height
    // We use "j" as the standard
    temp.setString(L"j");
    metrics.descenderLine = temp.getLocalBounds().height - metrics.baseLine;
    metrics.fullHeight = metrics.ascenderLine + metrics.baseLine + metrics.descenderLine;

    return metrics;
}

Theme::Theme(const TextSettings& textSettings,
             const DecorationSettings& idleColorSettings,
             const DecorationSettings& hoverColorSettings,
             const DecorationSettings& pressColorSettings) :
    m_textSettings(textSettings),
    m_idleColorSettings(idleColorSettings),
    m_hoverColorSettings(hoverColorSettings),
    m_pressColorSettings(pressColorSettings)
{
    //ctor
}

Theme::~Theme()
{
    //dtor
}

const TextSettings& Theme::getTextSettings() const
{
    return m_textSettings;
}

const DecorationSettings& Theme::getIdleColorSettings() const
{
    return m_idleColorSettings;
}

const DecorationSettings& Theme::getHoveredColorSettings() const
{
    return m_hoverColorSettings;
}

const DecorationSettings& Theme::getPressedColorSettings() const
{
    return m_pressColorSettings;
}

WidgetPool::WidgetPool() :
    m_window(nullptr),
    m_activeWidget(nullptr),
    m_lastHoveredWidget(nullptr),
    m_lastClickedWidget(nullptr)
{
    //ctor
}

WidgetPool::~WidgetPool()
{
    //dtor
}

WidgetPool& WidgetPool::getInstance()
{
    static WidgetPool instance;
    return instance;
}

void WidgetPool::initialize(const sf::RenderWindow* window)
{
    m_window = window;
}

void WidgetPool::addWidget(Widget* widget)
{
    m_widgets.push_back(widget);
}

void WidgetPool::processEvent(const sf::Event event)
{
    const auto position = m_window->mapPixelToCoords((sf::Mouse::getPosition(*m_window)));
    m_activeWidget = getActiveWidget(position);

    // Process events
    if (m_activeWidget != nullptr)
        m_activeWidget->processEvent(event, position);

    if (m_lastClickedWidget != nullptr && m_lastClickedWidget != m_activeWidget)
        m_lastClickedWidget->processEvent(event, position);

    if (m_lastHoveredWidget != nullptr && m_lastHoveredWidget != m_activeWidget && m_lastHoveredWidget != m_lastClickedWidget)
        m_lastHoveredWidget->processEvent(event, position);

    if (event.type == sf::Event::MouseButtonReleased)
        m_lastClickedWidget = m_activeWidget;

    m_lastHoveredWidget = m_activeWidget;
}

Widget* WidgetPool::getActiveWidget(const sf::Vector2f& mousePosition) const
{
    auto isSelected = [&mousePosition](Widget* w) { return !w->isHidden() && w->getGlobalBounds().contains(mousePosition); };

    // Widgets can be drawn upon each other (it should not be made by the user intentionally)
    // Anyway, the later we draw them, the bigger index they have
    // We need to select the top widget
    auto it = std::find_if(std::reverse_iterator(m_widgets.cend()), std::reverse_iterator(m_widgets.cbegin()), isSelected);
    return it != std::reverse_iterator(m_widgets.cbegin()) ? *it : nullptr;
}

void WidgetPool::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& widget : m_widgets)
    {
        if (!widget->isHidden())
            target.draw(*widget);
    }
}

Widget::Widget() : m_theme(nullptr), m_padding(5.0f, 10.0f), m_state(WidgetState::Idle), m_contentNeedsUpdate(true)
{
    auto& ui = WidgetPool::getInstance();
    ui.addWidget(this);
}

Widget::~Widget()
{
    // dtor
}

void Widget::setPosition(const sf::Vector2f& position)
{
    m_rectangle.setPosition(position);
    m_contentNeedsUpdate = true;
}

void Widget::setSize(const sf::Vector2f& size)
{
    m_rectangle.setSize(size);
    m_contentNeedsUpdate = true;
}

void Widget::setTheme(const Theme& theme)
{
    m_theme = &theme;
    m_contentNeedsUpdate = true;
}

void Widget::setPadding(const sf::Vector2f& padding)
{
    m_padding = padding;
    m_contentNeedsUpdate = true;
}

void Widget::setBackgroundTextureRect(const sf::IntRect& rectangle)
{
    m_rectangle.setTextureRect(rectangle);
}

void Widget::setAction(const std::function <void()> doActionOnButtonRelease)
{
    m_doActionOnButtonRelease = doActionOnButtonRelease;
}

sf::Vector2f Widget::getPosition() const
{
    return m_rectangle.getPosition();
}

sf::Vector2f Widget::getSize() const
{
    return m_rectangle.getSize();
}

const Theme& Widget::getTheme() const
{
    return *m_theme;
}

sf::Vector2f Widget::getPadding() const
{
    return m_padding;
}

sf::IntRect Widget::getBackgroundTextureRect() const
{
    return m_rectangle.getTextureRect();
}

sf::FloatRect Widget::getLocalBounds() const
{
    return m_rectangle.getLocalBounds();
}

sf::FloatRect Widget::getGlobalBounds() const
{
    return m_rectangle.getGlobalBounds();
}

void Widget::show()
{
    m_state = WidgetState::Idle;
    m_contentNeedsUpdate = true;
}

void Widget::hide()
{
    m_state = WidgetState::Hidden;
}

bool Widget::isHidden() const
{
    return m_state == WidgetState::Hidden;
}

void Widget::forceStylesUpdate() const
{
    m_contentNeedsUpdate = true;
}

void Widget::refreshStyles() const
{
    const DecorationSettings* colorSettings;
    switch (m_state)
    {
        case WidgetState::Idle:
            colorSettings = &(m_theme->getIdleColorSettings());
            break;

        case WidgetState::Hovered:
            colorSettings = &(m_theme->getHoveredColorSettings());
            break;

        case WidgetState::Pressed:
            colorSettings = &(m_theme->getPressedColorSettings());
            break;

        default:
            break;
    }

    m_rectangle.setFillColor(colorSettings->getFillColor());
    m_rectangle.setOutlineThickness(colorSettings->getOutlineThickness());
    m_rectangle.setOutlineColor(colorSettings->getOutlineColor());
    m_rectangle.setTexture(colorSettings->getBackgroundTexture());
}

void Widget::changeState(const WidgetState state)
{
    m_state = state;
    refreshStyles();
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // pure virtual
}

void Widget::processEvent(const sf::Event event, const sf::Vector2f& mousePosition)
{
    if (m_state == WidgetState::Hidden)
        return;

    if (!m_rectangle.getGlobalBounds().contains(mousePosition))
    {
        changeState(WidgetState::Idle);
        return;
    }

    switch (event.type)
    {
        case sf::Event::MouseLeft:
        {
            changeState(WidgetState::Idle);
            break;
        }

        case sf::Event::MouseButtonPressed:
        {
            changeState(WidgetState::Pressed);
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            if (m_doActionOnButtonRelease != nullptr)
                m_doActionOnButtonRelease();

            changeState(WidgetState::Hovered);
            break;
        }

        case sf::Event::MouseMoved:
        {
            // Prevent styles update on each frame
            if (m_state == WidgetState::Hovered)
                break;

            // Prevent style change if the widget is not released
            if (m_state == WidgetState::Pressed)
                break;

            changeState(WidgetState::Hovered);
            break;
        }

        default:
            break;
    }
}

TextBasedWidget::TextBasedWidget() : Widget(), m_isMultiline(false)
{
    //ctor
}

TextBasedWidget::~TextBasedWidget()
{
    //dtor
}

void TextBasedWidget::setPosition(const sf::Vector2f& position)
{
    m_rectangle.setPosition(position);
    m_contentNeedsUpdate = true;
}

void TextBasedWidget::refreshStyles() const
{
    const auto& textSettings = m_theme->getTextSettings();

    const DecorationSettings* colorSettings;
    switch (m_state)
    {
        case WidgetState::Idle:
            colorSettings = &(m_theme->getIdleColorSettings());
            break;

        case WidgetState::Hovered:
            colorSettings = &(m_theme->getHoveredColorSettings());
            break;

        case WidgetState::Pressed:
            colorSettings = &(m_theme->getPressedColorSettings());
            break;

        default:
            break;
    }

    m_rectangle.setFillColor(colorSettings->getFillColor());
    m_rectangle.setOutlineThickness(colorSettings->getOutlineThickness());
    m_rectangle.setOutlineColor(colorSettings->getOutlineColor());
    m_rectangle.setTexture(colorSettings->getBackgroundTexture());

    for (auto& line : m_lines)
    {
        line.setFont(textSettings.getFont());
        line.setCharacterSize(textSettings.getCharacterSize());
        line.setFillColor(colorSettings->getTextColor());
        line.setStyle(colorSettings->getTextStyle());
    }
}

sf::String TextBasedWidget::getString() const
{
    return m_string;
}

bool TextBasedWidget::getMultiline() const
{
    return m_isMultiline;
}

void TextBasedWidget::setSizeFitToText()
{
    updateTextSplitting();
    refreshStyles();

    float width = m_padding.x * 2;

    const auto longestLine = std::max_element(m_lines.cbegin(), m_lines.cend(), [](const sf::Text& a, const sf::Text& b)
    {
        return (a.getLocalBounds().left + a.getLocalBounds().width) < (b.getLocalBounds().left + b.getLocalBounds().width);
    });

    if (longestLine != m_lines.cend())
        width += (longestLine->getLocalBounds().left + longestLine->getLocalBounds().width);

    float height = m_padding.y * 2;
    height += m_theme->getTextSettings().getFontMetrics().fullHeight * m_lines.size();

    setSize({ width, height });
    placeText();

    m_contentNeedsUpdate = false;
}

void TextBasedWidget::setString(const sf::String& text)
{
    m_string = text;
    m_contentNeedsUpdate = true;
}

void TextBasedWidget::setMultiline(bool isMultiline)
{
    m_isMultiline = isMultiline;
    m_contentNeedsUpdate = true;
}

void TextBasedWidget::updateTextSplitting() const
{
    m_lines.clear();

    if (!m_isMultiline)
    {
        m_lines.emplace_back();
        m_lines.back().setString(m_string);
        return;
    }

    const auto maxWidth = m_rectangle.getSize().x - 2 * m_padding.x;

    const auto& textSettings = m_theme->getTextSettings();
    sf::Text textLine;
    textLine.setFont(textSettings.getFont());
    textLine.setCharacterSize(textSettings.getCharacterSize());

    std::vector <sf::String> words;

    for (size_t beginPosition = 0, endPosition = 0; beginPosition < m_string.getSize();)
    {
        sf::String word;
        endPosition = m_string.find(sf::String(L" "), beginPosition);

        if (endPosition == sf::String::InvalidPos)
        {
            word = m_string.substring(beginPosition);
            endPosition = m_string.getSize();
        }
        else
            word = m_string.substring(beginPosition, endPosition - beginPosition);

        // Split very long words that do not fit the rectangle at all
        textLine.setString(word);

        if (textLine.getLocalBounds().left + textLine.getLocalBounds().width > maxWidth)
        {
            word.clear();
            textLine.setString(word);

            for (; beginPosition < endPosition; beginPosition++)
            {
                word += m_string[beginPosition];
                textLine.setString(word);

                if (textLine.getLocalBounds().width > maxWidth)
                {
                    word.erase(word.getSize() - 1, 1);
                    break;
                }
            }
        }
        else
            beginPosition = endPosition + 1;

        words.push_back(word);
    }

    sf::String line;
    bool isLineReady = false;

    for (size_t i = 0; i < words.size();)
    {
        if (line.getSize() != 0)
            line += sf::String(L" ");
        line += words[i];

        textLine.setString(line);

        if (textLine.getLocalBounds().left + textLine.getLocalBounds().width > maxWidth)
        {
            line = line.substring(0, line.getSize() - words[i].getSize());
            isLineReady = true;
        }
        else if (i + 1 == words.size())
        {
            isLineReady = true;
            i++;
        }
        else
            i++;

        if (isLineReady)
        {
            const auto spaceSymbol = sf::String(L" ")[0];

            size_t trimLeft = 0;
            for (; trimLeft < line.getSize() && line[trimLeft] == spaceSymbol; )
                trimLeft++;
            line = line.substring(trimLeft);

            size_t trimRight = line.getSize() - 1;
            for (size_t j = trimRight + 1; j > 0 && line[trimRight] == spaceSymbol; j--)
                trimRight--;
            line = line.substring(0, trimRight + 1);

            m_lines.emplace_back();
            m_lines.back().setString(line);

            line.clear();
            isLineReady = false;
        }
    }
}

void TextBasedWidget::placeText() const
{
    const auto position = m_rectangle.getPosition();
    const auto size = m_rectangle.getSize();

    const auto& textSettings = m_theme->getTextSettings();
    const auto horizontalAlignment = textSettings.getHorizontalAlignment();
    const auto verticalAlignment = textSettings.getVerticalAlignment();
    const auto& metrics = textSettings.getFontMetrics();

    sf::Vector2f textPosition;

    for (size_t i = 0; i < m_lines.size(); i++)
    {
        switch (horizontalAlignment)
        {
            case TextHorizontalAlignment::Left:
                textPosition.x = position.x + m_padding.x;
                break;

            case TextHorizontalAlignment::Center:
                textPosition.x = position.x + (size.x - m_lines[i].getLocalBounds().width) / 2.0f - m_lines[i].getLocalBounds().left;
                break;

            case TextHorizontalAlignment::Right:
                textPosition.x = position.x + size.x - m_padding.x - m_lines[i].getLocalBounds().width;
                break;
        }

        switch (verticalAlignment)
        {
            case TextVerticalAlignment::Top:
                textPosition.y = position.y + m_padding.y + i * metrics.fullHeight;
                break;

            case TextVerticalAlignment::Center:
                textPosition.y = position.y + (size.y - m_lines.size() * metrics.fullHeight - metrics.ascenderLine + metrics.descenderLine) / 2.0f + i * metrics.fullHeight;
                break;

            case TextVerticalAlignment::Bottom:
                textPosition.y = position.y + size.y - m_padding.y - (m_lines.size() - i) * metrics.fullHeight - metrics.ascenderLine + metrics.descenderLine;
                break;
        }

        // If coordinates are not integer, the text gets blurred
        textPosition.x = std::round(textPosition.x);
        textPosition.y = std::round(textPosition.y);
        m_lines[i].setPosition(textPosition);
    }
}

void TextBasedWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_state == WidgetState::Hidden)
        return;

    if (m_contentNeedsUpdate)
    {
        updateTextSplitting();
        refreshStyles();
        placeText();
        m_contentNeedsUpdate = false;
    }

    target.draw(m_rectangle);

    // Clip text
    auto bounds = m_rectangle.getGlobalBounds();
    bounds.left += m_padding.x;
    bounds.top += m_padding.y;
    bounds.width -= m_padding.x * 2;
    bounds.height -= m_padding.y * 2;

    const auto size = target.mapPixelToCoords(static_cast <sf::Vector2i>(target.getSize()));
    sf::View view(bounds);
    view.setViewport(sf::FloatRect(bounds.left / size.x, bounds.top / size.y,
                                   bounds.width / size.x, bounds.height / size.y));

    const auto oldView = target.getView();
    target.setView(view);

    for (const auto& line : m_lines)
        target.draw(line);

    target.setView(oldView);
}

PushButton::PushButton() : TextBasedWidget()
{
    //ctor
}

PushButton::~PushButton()
{
    //dtor
}

IconButton::IconButton() : Widget()
{
    setPadding({0.0f, 0.0f});
}

IconButton::~IconButton()
{
    //dtor
}

void IconButton::setPosition(const sf::Vector2f& position)
{
    m_rectangle.setPosition(position);
    m_icon.setPosition(position);
    m_contentNeedsUpdate = true;
}

void IconButton::setSize(const sf::Vector2f& size)
{
    m_rectangle.setSize(size);

    // Icon resize is delayed because we cannot ensure that the programmer use methods in the right order
    m_contentNeedsUpdate = true;
}

void IconButton::setIconTexture(const sf::Texture& texture)
{
    m_icon.setTexture(texture);
    m_contentNeedsUpdate = true;
}

void IconButton::setIconTextureRect(const sf::IntRect& rectangle)
{
    m_icon.setTextureRect(rectangle);
    m_contentNeedsUpdate = true;
}

const sf::Texture* IconButton::getIconTexture() const
{
    return m_icon.getTexture();
}

sf::IntRect IconButton::getIconTextureRect() const
{
    return m_icon.getTextureRect();
}

void IconButton::updateSpriteSize() const
{
    const auto bounds = m_icon.getLocalBounds();
    const auto size = m_rectangle.getSize();

    sf::Vector2f factor;

    factor.x = size.x / (bounds.width + bounds.left);
    factor.y = size.y / (bounds.height + bounds.top);

    m_icon.setScale(factor);
}

void IconButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_state == WidgetState::Hidden)
        return;

    if (m_contentNeedsUpdate)
        updateSpriteSize();

    target.draw(m_rectangle);
    target.draw(m_icon);
}

DropDownList::DropDownList() : TextBasedWidget(), m_isOpened(false)
{
    //ctor
}

DropDownList::~DropDownList()
{
    //dtor
}

void DropDownList::addListItem(const sf::String& label, const std::function <void()> doAction)
{
    const auto x = getPosition().x;
    const auto y = (m_items.empty() ?
                    getGlobalBounds().top + getGlobalBounds().height :
                    m_items.back().getGlobalBounds().top + m_items.back().getGlobalBounds().height);

    m_items.emplace_back();

    m_items.back().setString(label);
    m_items.back().setAction(doAction);
    m_items.back().setTheme(*m_theme);
    m_items.back().setPadding(m_padding);
    m_items.back().setPosition({x, y});
    m_items.back().setSizeFitToText();

    const auto longestItem = max_element(m_items.cbegin(), m_items.cend(), [](const PushButton& a, const PushButton& b)
    {
        return a.getSize().x < b.getSize().x;
    });

    const auto width = longestItem->getSize().x;
    const auto height = longestItem->getSize().y;

    for (auto& item : m_items)
        item.setSize({width, height});

    m_items.back().hide();
}

void DropDownList::showItems()
{
    for (auto& item : m_items)
        item.show();
    m_isOpened = true;
}

void DropDownList::hideItems()
{
    for (auto& item : m_items)
        item.hide();
    m_isOpened = false;
}

void DropDownList::processEvent(const sf::Event event, const sf::Vector2f& mousePosition)
{
    if (m_state == WidgetState::Hidden)
        return;

    const auto isMouseInside = m_rectangle.getGlobalBounds().contains(mousePosition);

    switch (event.type)
    {
        case sf::Event::MouseLeft:
        {
            changeState(WidgetState::Idle);
            if (m_isOpened)
                hideItems();
            break;
        }

        case sf::Event::MouseButtonPressed:
        {
            if (isMouseInside && m_state != WidgetState::Pressed)
                changeState(WidgetState::Pressed);
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            if (m_doActionOnButtonRelease != nullptr)
                m_doActionOnButtonRelease();

            if (isMouseInside)
            {
                if (m_state == WidgetState::Pressed)
                {
                    if (m_isOpened)
                    {
                        changeState(WidgetState::Hovered);
                        hideItems();
                    }
                    else
                        showItems();
                }
            }
            else
            {
                changeState(WidgetState::Idle);
                if (m_isOpened)
                    hideItems();
            }

            break;
        }

        case sf::Event::MouseMoved:
        {
            if (m_state == WidgetState::Hovered)
            {
                if (!isMouseInside)
                    changeState(WidgetState::Idle);
            }
            else if (m_state == WidgetState::Idle)
            {
                if (isMouseInside)
                    changeState(WidgetState::Hovered);
            }
            else if (m_state == WidgetState::Pressed)
            {
                if (!isMouseInside && !m_isOpened)
                    changeState(WidgetState::Idle);
            }

            break;
        }

        default:
            break;
    }
}

}   // namespace SfGui
