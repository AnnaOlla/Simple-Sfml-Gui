#include "SmallSfmlGui.hpp"

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(600, 300), "Text input example");
    window.setFramerateLimit(30);

    // Then create an instance of user interface handler
    auto& ui = SmallGui::WidgetPool::getInstance();
    ui.initialize(&window);

    sf::RectangleShape background;
    background.setFillColor(sf::Color(220, 220, 220));
    background.setPosition(sf::Vector2f(0, 0));
    background.setSize(sf::Vector2f(600, 300));

    // Load a font. Make sure that the font file is put in the project directory (the author does not provide it)
    sf::Font font;
    const auto loadResult = font.loadFromFile("arial.ttf");
    if (!loadResult)
        return 0;

    // Create an input area
    SmallGui::TextSettings textInputStyle(font, 36, SmallGui::TextHorizontalAlignment::Left, SmallGui::TextVerticalAlignment::Top);
    SmallGui::DecorationSettings idleInputStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 2.0f, sf::Color(128, 128, 128));
    SmallGui::DecorationSettings hoveredInputStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 2.0f, sf::Color::Black);
    SmallGui::DecorationSettings pressedInputStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 2.0f, sf::Color(0, 120, 215));
    SmallGui::Theme textInputTheme(textInputStyle, idleInputStyle, hoveredInputStyle, pressedInputStyle);

    SmallGui::TextBox textInput;
    textInput.setTheme(textInputTheme);
    textInput.setPosition(sf::Vector2f(10, 55));
    textInput.setSize(sf::Vector2f(580, 225));
    textInput.setMultilined(true);

    // Create a menu
    SmallGui::TextSettings menuText(font, 16, SmallGui::TextHorizontalAlignment::Left);
    SmallGui::DecorationSettings idleMenu(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 0.0f);
    SmallGui::DecorationSettings hoveredMenu(sf::Color(229, 243, 255), sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(204, 232, 255));
    SmallGui::DecorationSettings pressedMenu(sf::Color(204, 232, 255), sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color(153, 209, 255));
    SmallGui::Theme menuTheme(menuText, idleMenu, hoveredMenu, pressedMenu);

    // It is possible to reuse settings and/or apply the same ones
    SmallGui::DecorationSettings idleMenuItem(sf::Color(242, 242, 242), sf::Color::Black, sf::Text::Style::Regular, 0.0f);
    SmallGui::DecorationSettings activeMenuItem(sf::Color(145, 201, 247), sf::Color::Black, sf::Text::Style::Regular, 2.0f, sf::Color(242, 242, 242));
    SmallGui::Theme menuItemTheme(menuText, idleMenuItem, activeMenuItem, activeMenuItem);

    SmallGui::DropDownList fileMenu;
    fileMenu.setTheme(menuTheme);
    fileMenu.setString(L"File");
    fileMenu.setSizeFitToText();                // Auto-size feature
    fileMenu.setPosition(sf::Vector2f(0, 0));

    fileMenu.setItemsTheme(menuItemTheme);
    fileMenu.addListItem(L"New", [&textInput]() { textInput.setString(L""); } );
    fileMenu.addListItem(L"Add some string", [&textInput]() { textInput.setString(textInput.getString() + L"some string"); } );
    fileMenu.addListItem(L"Set limit to 50 symbols", [&textInput]() { textInput.setMaxInputLength(50); } );
    fileMenu.addListItem(L"Switch Multiline", [&textInput]() { textInput.setMultilined(!textInput.isMultiline()); });
    fileMenu.addListItem(L"Close", [&window]() { window.close(); } );

    SmallGui::DropDownList helpMenu;
    helpMenu.setTheme(menuTheme);
    helpMenu.setString(L"Help");
    helpMenu.setSizeFitToText();
    helpMenu.setPosition(sf::Vector2f(fileMenu.getPosition().x + fileMenu.getSize().x, 0));

    helpMenu.setItemsTheme(menuItemTheme);
    helpMenu.setAction(sf::Event::MouseButtonReleased, [&helpMenu]() { helpMenu.setString(L"There is no help"); helpMenu.setSizeFitToText(); });

    // Since there is no automatic menubar, draw it by yourself
    sf::RectangleShape menubar;
    menubar.setFillColor(sf::Color::White);
    menubar.setPosition(sf::Vector2f(0, 0));
    menubar.setSize(sf::Vector2f(600, fileMenu.getSize().y));

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
        window.draw(menubar);
        window.draw(ui);

        window.display();
    }

    return 0;
}
