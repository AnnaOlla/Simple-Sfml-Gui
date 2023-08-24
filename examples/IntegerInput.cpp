#include "SmallSfmlGui.hpp"

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(520, 800), "Symbol Table Explorer");
    window.setFramerateLimit(30);

    // Then create an instance of user interface handler
    auto& ui = SmallGui::WidgetPool::getInstance();
    ui.initialize(&window);

    // Load the font. Make sure that you have added it to the folder
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return 0;

    // Load icons. Make sure that the author's file has been uploaded with the example
    sf::Texture icons;
    if (!icons.loadFromFile("resources.png"))
        return -1;

    // Create themes
    SmallGui::TextSettings largeTextSettings(font, 384);
    SmallGui::TextSettings textSettings(font, 64);

    SmallGui::DecorationSettings iconIdleStyle(sf::Color::Transparent, sf::Color::Black, sf::Text::Style::Regular, 0.0f);
    SmallGui::DecorationSettings iconHoveredStyle(sf::Color(229, 243, 255), sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(204, 232, 255));
    SmallGui::DecorationSettings iconPressedStyle(sf::Color(204, 232, 255), sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(153, 209, 255));

    SmallGui::DecorationSettings textIdleStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(128, 128, 128));
    SmallGui::DecorationSettings textHoveredStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color::Black);
    SmallGui::DecorationSettings textPressedStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(0, 120, 215));

    SmallGui::Theme iconTheme(textSettings, iconIdleStyle, iconHoveredStyle, iconPressedStyle);
    SmallGui::Theme textTheme(textSettings, textIdleStyle, textHoveredStyle, textPressedStyle);
    SmallGui::Theme largeTextTheme(largeTextSettings, textIdleStyle, textHoveredStyle, textPressedStyle);

    // Create a background
    sf::RectangleShape background;
    background.setFillColor(sf::Color::White);
    background.setPosition(sf::Vector2f(0, 0));
    background.setSize(static_cast <sf::Vector2f>(window.getSize()));

    // Create GUI elements
    SmallGui::IconButton previousCharacterButton;
    previousCharacterButton.setTheme(iconTheme);
    previousCharacterButton.setIconTexture(icons);
    previousCharacterButton.setIconTextureRect({0, 64, 64, 64});
    previousCharacterButton.setPosition({20, 20});
    previousCharacterButton.setSize({128, 128});          // It is not mandatory to use the same size that the texture rectangle has

    SmallGui::TextBox characterCodeBox;
    characterCodeBox.setTheme(textTheme);
    characterCodeBox.setPosition({148, 20});
    characterCodeBox.setSize({224, 128});
    characterCodeBox.setMaxInputLength(5);

    SmallGui::IconButton nextCharacterButton;
    nextCharacterButton.setTheme(iconTheme);
    nextCharacterButton.setIconTexture(icons);
    nextCharacterButton.setIconTextureRect({64, 64, 64, 64});
    nextCharacterButton.setPosition({372, 20});
    nextCharacterButton.setSize({128, 128});

    SmallGui::StaticText symbolBox;
    symbolBox.setTheme(largeTextTheme);
    symbolBox.setPosition({20, 188});
    symbolBox.setSize({480, 592});

    // Create symbol to show
    sf::Uint32 symbolCode = 'A';
    symbolBox.setString(symbolCode);
    characterCodeBox.setString(std::to_string(symbolCode));

    // Add event listeners
    previousCharacterButton.setAction(sf::Event::MouseButtonReleased, [&symbolCode, &symbolBox, &characterCodeBox]()
    {
        symbolCode--;
        symbolBox.setString(symbolCode);
        characterCodeBox.setString(std::to_string(symbolCode));
    });

    characterCodeBox.setAction(sf::Event::TextEntered, [&symbolCode, &symbolBox, &characterCodeBox]()
    {
        try
        {
            const auto stringified = static_cast <std::string>(characterCodeBox.getString());
            symbolCode = stringified.empty() ? 0 : std::stoi(stringified);
        }
        catch (const std::exception&)
        {
            // Do nothing
        }

        symbolBox.setString(symbolCode);
        characterCodeBox.setString(std::to_string(symbolCode));
    });

    nextCharacterButton.setAction(sf::Event::MouseButtonReleased, [&symbolCode, &symbolBox, &characterCodeBox]()
    {
        symbolCode++;
        symbolBox.setString(symbolCode);
        characterCodeBox.setString(std::to_string(symbolCode));
    });

    // Run the application
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Process events by GUI
            ui.processEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw the interface
        window.draw(background);
        window.draw(ui);

        window.display();
    }

    return 0;
}
