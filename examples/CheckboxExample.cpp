#include "SmallSfmlGui.hpp"

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(600, 150), "Checkbox");
    window.setFramerateLimit(30);

    // Then create an instance of user interface handler
    auto& ui = SmallGui::WidgetPool::getInstance();
    ui.initialize(&window);

    // Load the font. Make sure that you have added it to the folder
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return 0;

    // Theme settings
    sf::Color mainColor(43, 45, 49);
    sf::Color secondaryColor(72, 82, 89);

    SmallGui::DecorationSettings staticsStyle(mainColor, sf::Color::White, sf::Text::Style::Regular, 0);
    SmallGui::DecorationSettings idleStyle(mainColor, sf::Color::White, sf::Text::Style::Regular, 1, secondaryColor);
    SmallGui::DecorationSettings activeStyle(secondaryColor, sf::Color::White, sf::Text::Style::Regular, 1, secondaryColor);

    SmallGui::TextSettings textboxTextStyle(font, 32, SmallGui::TextHorizontalAlignment::Left);
    SmallGui::TextSettings buttonsTextStyle(font, 32);

    SmallGui::Theme buttonsTheme(buttonsTextStyle, idleStyle, activeStyle, activeStyle);
    SmallGui::Theme staticsTheme(buttonsTextStyle, staticsStyle, staticsStyle, staticsStyle);
    SmallGui::Theme textboxTheme(textboxTextStyle, idleStyle, activeStyle, activeStyle);

    // Draw a background
    sf::RectangleShape background;
    background.setFillColor(mainColor);
    background.setPosition(sf::Vector2f(0, 0));
    background.setSize(static_cast <sf::Vector2f>(window.getSize()));

    // Create a checkbox
    SmallGui::CheckBox checkbox;
    checkbox.setTheme(buttonsTheme);
    checkbox.setPosition(sf::Vector2f(10, 10));
    checkbox.setSizeFitToText();

    // Create a label for the checkbox
    SmallGui::StaticText staticText;
    staticText.setTheme(staticsTheme);
    staticText.setPosition(sf::Vector2f(checkbox.getPosition().x + checkbox.getSize().x, checkbox.getPosition().y));
    staticText.setString(L"Use custom checkmark");
    staticText.setSizeFitToText();

    // Create a textbox to change checkmark
    SmallGui::TextBox textbox;
    textbox.setTheme(textboxTheme);
    textbox.setPosition(sf::Vector2f(checkbox.getPosition().x, checkbox.getPosition().y + checkbox.getSize().y + 10));
    textbox.setString(checkbox.getString());
    textbox.setSize(sf::Vector2f(450, staticText.getSize().y));
    textbox.setMaxInputLength(10);
    textbox.hide();

    // Create a button to apply new checkmarks
    SmallGui::PushButton button;
    button.setTheme(buttonsTheme);
    button.setPosition(sf::Vector2f(textbox.getPosition().x + textbox.getSize().x + 10, textbox.getPosition().y));
    button.setSize(sf::Vector2f(window.getSize().x - button.getPosition().x - 10, textbox.getSize().y));
    button.setString(L"Apply");
    button.hide();

    // Set the actions for the checkbox:
    checkbox.setAction([&textbox, &checkbox, &button]()
    {
        // If we want to set a custom checkmark, show elements to customize it
        if (checkbox.isChecked())
        {
            textbox.show();
            button.show();
        }

        // Else hide them
        else
        {
            textbox.hide();
            button.hide();
        }
    });

    // Set the action to apply the new checkmark
    button.setAction([&textbox, &checkbox, &staticText]()
    {
        checkbox.setString(textbox.getString());
        checkbox.setSizeFitToText();
        staticText.setPosition(sf::Vector2f(checkbox.getPosition().x + checkbox.getSize().x, checkbox.getPosition().y));
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
