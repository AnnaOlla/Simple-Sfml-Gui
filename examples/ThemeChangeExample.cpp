#include "SmallSfmlGui.hpp"

#include <fstream>

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Small Notepad");
    window.setFramerateLimit(30);

    // Then create an instance of user interface handler
    auto& ui = SmallGui::WidgetPool::getInstance();
    ui.initialize(&window);

    // Create the background
    sf::RectangleShape background;
    background.setFillColor({220, 220, 220});
    background.setPosition({0, 0});
    background.setSize({620, 620});

    // Load a font. Make sure that the font file is put in the project directory (the author does not provide it)
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return -1;

    // Create themes
    SmallGui::TextSettings textAreaSettings(font, 36, SmallGui::TextHorizontalAlignment::Left, SmallGui::TextVerticalAlignment::Top);
    SmallGui::TextSettings textSettings(font, 36, SmallGui::TextHorizontalAlignment::Left);

    SmallGui::DecorationSettings iconIdleStyle(sf::Color::Transparent, sf::Color::Black, sf::Text::Style::Regular, 0.0f);
    SmallGui::DecorationSettings iconHoveredStyle(sf::Color(229, 243, 255), sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(204, 232, 255));
    SmallGui::DecorationSettings iconPressedStyle(sf::Color(204, 232, 255), sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(153, 209, 255));
    SmallGui::Theme iconTheme(textSettings, iconIdleStyle, iconHoveredStyle, iconPressedStyle);

    SmallGui::DecorationSettings textIdleStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(128, 128, 128));
    SmallGui::DecorationSettings textHoveredStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color::Black);
    SmallGui::DecorationSettings textPressedStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(0, 120, 215));
    SmallGui::Theme textAreaTheme(textAreaSettings, textIdleStyle, textHoveredStyle, textPressedStyle);
    SmallGui::Theme fileNameTheme(textSettings, textIdleStyle, textHoveredStyle, textPressedStyle);

    // Load icons. Make sure that the author's file has been uploaded with the example
    sf::Texture icons;
    if (!icons.loadFromFile("resources.png"))
        return -1;

    // Create elements
    const auto iconSize = 64;

    sf::RectangleShape iconBar;
    iconBar.setPosition({0.0f, 0.0f});
    iconBar.setSize({620.0f, iconSize});
    iconBar.setFillColor({240, 240, 240});

    SmallGui::IconButton newFileButton;
    newFileButton.setPosition({iconSize * 0, 0.0f});
    newFileButton.setSize({iconSize, iconSize});
    newFileButton.setIconTexture(icons);
    newFileButton.setIconTextureRect({0, 0, iconSize, iconSize});
    newFileButton.setTheme(iconTheme);

    SmallGui::IconButton saveFileButton;
    saveFileButton.setPosition({iconSize * 1, 0.0f});
    saveFileButton.setSize({iconSize, iconSize});
    saveFileButton.setIconTexture(icons);
    saveFileButton.setIconTextureRect({iconSize, 0, iconSize, iconSize});
    saveFileButton.setTheme(iconTheme);

    SmallGui::IconButton decreaseSizeButton;
    decreaseSizeButton.setPosition({iconSize * 3, 0.0f});
    decreaseSizeButton.setSize({iconSize, iconSize});
    decreaseSizeButton.setIconTexture(icons);
    decreaseSizeButton.setIconTextureRect({0, iconSize * 2, iconSize, iconSize});
    decreaseSizeButton.setTheme(iconTheme);

    SmallGui::IconButton increaseSizeButton;
    increaseSizeButton.setPosition({iconSize * 2, 0.0f});
    increaseSizeButton.setSize({iconSize, iconSize});
    increaseSizeButton.setIconTexture(icons);
    increaseSizeButton.setIconTextureRect({iconSize, iconSize * 2, iconSize, iconSize});
    increaseSizeButton.setTheme(iconTheme);

    SmallGui::TextBox fileNameBox;
    fileNameBox.setPosition({iconSize * 4, 0});
    fileNameBox.setSize({static_cast <float> (window.getSize().x) - iconSize * 4, iconSize});
    fileNameBox.setTheme(fileNameTheme);
    fileNameBox.setString(L"save.txt");

    SmallGui::TextBox textArea;
    textArea.setPosition({0, iconSize});
    textArea.setSize({static_cast <float> (window.getSize().x), static_cast <float> (window.getSize().y) - iconSize});
    textArea.setTheme(textAreaTheme);
    textArea.setMultilined(true);

    // Set actions for buttons
    newFileButton.setAction([&textArea]() { textArea.setString(L""); });

    saveFileButton.setAction([&textArea, &fileNameBox]()
    {
        const auto fileName = fileNameBox.getString().toAnsiString();
        if (fileName.empty())
            return;

        std::ofstream fout(fileName);
        if (!fout)
            return;

        fout << textArea.getString().toAnsiString();
    });

    increaseSizeButton.setAction([&textAreaSettings, &ui]()
    {
        // Widgets does not know automatically that something in DecorationSettings or TextSettings has been changed
        // This is why forcing update is necessary

        textAreaSettings.setCharacterSize(textAreaSettings.getCharacterSize() + 4);
        ui.forceThemeUpdate();
    });

    decreaseSizeButton.setAction([&textAreaSettings, &ui]()
    {
        textAreaSettings.setCharacterSize(textAreaSettings.getCharacterSize() - 4);
        ui.forceThemeUpdate();
    });

    // Run the application
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            ui.processEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.draw(background);
        window.draw(iconBar);
        window.draw(ui);

        window.display();
    }

    return 0;
}
