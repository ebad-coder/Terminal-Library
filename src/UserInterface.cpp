#include "../include/UserInterface.h"
#include <sstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

static std::string message_buffer = "";


/**************************************************************************************/


// shows main menu to user, where user can select which role (admin/member) to continue with
int UserInterface::show_main_menu()
{
    std::vector<std::string> entries = {
        "I am a member",
        "I am an admin",
        "Exit"
    };
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? ">  " : "   ";
        std::string suffix = state.active ? "  <" : "   ";
        
        Element e = text(prefix + state.label + suffix) | center;

        if (state.active) {
            return e | bold | color(Color::CyanLight) | inverted;
        }
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
            separatorEmpty(),
            text(" LIBRARY RESOURCE MANAGEMENT SYSTEM ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            text("Please select your role to continue:") | dim | color(Color::White) | center,
            separatorEmpty(),
            menu->Render() | center,
            separatorEmpty(),
        }) 
        | borderRounded
        | color(Color::BlueLight)
        | size(WIDTH, GREATER_THAN, 55)
        | center;
    });

    screen.Loop(renderer);

    return selected + 1;
}


/**************************************************************************************/


// shows member dashboard, where member can select which operation to perform
int UserInterface::show_member_dashboard(Account* current_user)
{
    std::vector<std::string> entries = {
        "Search a Resource",
        "View Available Resources",
        "Borrow a Resource",
        "Return a Resource",
        "Reserve a Resource",
        "View Inbox",
        "View Borrowing History",
        "View Balance",
        "View Membership Status",
        "View Profile",
        "Donate a Resource",
        "Logout"
    };
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? ">  " : "   ";
        std::string suffix = state.active ? "  <" : "   ";
        Element e = text(prefix + state.label + suffix) | center;
        
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);
    int unread_count = current_user->get_unread_count();

    auto renderer = Renderer(menu, [&] {
        Elements layout;

        layout.push_back(separatorEmpty());
        layout.push_back(text(" MEMBER DASHBOARD ") | bold | color(Color::CyanLight) | center);
        layout.push_back(separatorLight() | color(Color::GrayDark));
        layout.push_back(separatorEmpty());
        
        layout.push_back(text("Welcome, " + current_user->get_fullname() + "!") | bold | center);
        
        if (unread_count > 0)
        {
            layout.push_back(separatorEmpty());
            layout.push_back(text("You have " + std::to_string(unread_count) + " new notification(s)") | color(Color::YellowLight) | blink | center);
        }

        layout.push_back(separatorEmpty());
        layout.push_back(menu->Render() | center);
        layout.push_back(separatorEmpty());

        return vbox(layout) | borderRounded | color(Color::BlueLight) | size(WIDTH, GREATER_THAN, 60)| center;
    });

    screen.Loop(renderer);

    return selected + 1;
}


/**************************************************************************************/


// shows admin dashboard, where admin can select which operation to perform
int UserInterface::show_admin_dashboard(Account* current_user)
{
    std::vector<std::string> entries = {
        "Add New Resource",
        "Update a Resource",
        "Remove a Resource",
        "View All Members",
        "Send a Notification",
        "View Donation Requests",
        "Generate Report",
        "Logout"
    };
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? ">  " : "   ";
        std::string suffix = state.active ? "  <" : "   ";
        Element e = text(prefix + state.label + suffix) | center;
        
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);

    int unread_count = current_user->get_unread_count();

    auto renderer = Renderer(menu, [&] {
        Elements layout;
        layout.push_back(separatorEmpty());
        layout.push_back(text(" ADMIN DASHBOARD ") | bold | color(Color::CyanLight) | center);
        layout.push_back(separatorLight() | color(Color::GrayDark));
        layout.push_back(separatorEmpty());
        
        layout.push_back(text("Welcome, " + current_user->get_fullname() + "!") | bold | center);
        
        if (unread_count > 0)
        {
            layout.push_back(separatorEmpty());
            layout.push_back(text("You have " + std::to_string(unread_count) + " new notification(s)") | color(Color::YellowLight) | blink | center);
        }

        layout.push_back(separatorEmpty());
        layout.push_back(menu->Render() | center);
        layout.push_back(separatorEmpty());

        return vbox(layout) 
            | borderRounded 
            | color(Color::Magenta)
            | size(WIDTH, GREATER_THAN, 60)
            | center;
    });

    screen.Loop(renderer);
    
    return selected + 1;
}


/**************************************************************************************/


// shows authentication menu for member, where they can register or login to their account
int UserInterface::show_member_authentication_menu()
{
    std::vector<std::string> entries = {
        "Create a new account",
        "Login to system",
        "Back to main menu"
    };
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? ">  " : "   ";
        std::string suffix = state.active ? "  <" : "   ";
        Element e = text(prefix + state.label + suffix) | center;
        
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
            separatorEmpty(),
            text(" CONTINUE AS A MEMBER ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            menu->Render() | center,
            separatorEmpty(),
        }) 
        | borderRounded 
        | color(Color::BlueLight)
        | size(WIDTH, GREATER_THAN, 50)
        | center;
    });

    screen.Loop(renderer);

    return selected + 1;
}


/**************************************************************************************/


// shows authentication menu to an admin, from which admin can choose to login to their account
int UserInterface::show_admin_authentication_menu()
{
    std::vector<std::string> entries = {
        "Login to system",
        "Back to main menu"
    };
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? ">  " : "   ";
        std::string suffix = state.active ? "  <" : "   ";
        Element e = text(prefix + state.label + suffix) | center;
        
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);

    auto renderer = Renderer(menu, [&] {
        return vbox({
            separatorEmpty(),
            text(" CONTINUE AS AN ADMIN ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            menu->Render() | center,
            separatorEmpty(),
        }) 
        | borderRounded 
        | color(Color::Magenta)
        | size(WIDTH, GREATER_THAN, 50)
        | center;
    });

    screen.Loop(renderer);

    return selected + 1;
}


/**************************************************************************************/


// Helper function used to inject pending messages above menus
auto build_menu_renderer = [](Component menu, const std::string& title)
{
    return Renderer(menu, [menu, title] {
        Elements layout;

        layout.push_back(separatorEmpty());
        layout.push_back(text(title) | bold | color(Color::CyanLight) | center);
        layout.push_back(separatorLight() | color(Color::GrayDark));
        layout.push_back(separatorEmpty());

        // Inject anything sitting in the message buffer
        if (!message_buffer.empty()) {
            Elements text_elements;
            std::istringstream stream(message_buffer);
            std::string line;
            while (std::getline(stream, line)) {
                if (line.empty() || line == "\r") text_elements.push_back(separatorEmpty());
                else text_elements.push_back(text(line)); 
            }
            layout.push_back(vbox(text_elements) | center);
            layout.push_back(separatorEmpty());
            layout.push_back(separatorLight() | color(Color::GrayDark));
            layout.push_back(separatorEmpty());
        }

        layout.push_back(menu->Render() | center);
        layout.push_back(separatorEmpty());

        return vbox(layout) 
            | borderRounded 
            | color(Color::BlueLight) 
            | size(WIDTH, GREATER_THAN, 55) 
            | center;
    });
};


/**************************************************************************************/


// Allows the admin to choose which type of resource they want to add
int UserInterface::show_resource_options()
{
    std::vector<std::string> entries = {"Article", "Book", "Journal", "Back to menu"};
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();
    
    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? "> " : "  ";
        std::string suffix = state.active ? " <" : "  ";
        Element e = text(prefix + state.label + suffix) | center;
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);
    screen.Loop(build_menu_renderer(menu, " SELECT RESOURCE TYPE "));
    message_buffer.clear(); // Clear buffer after consuming it
    return selected + 1;
}


/**************************************************************************************/


// Renders a unified search form with a radiobox for criteria and an input for the search keyword.
bool UserInterface::show_search_form(int& criteria_index, std::string& search_term)
{
    auto screen = ScreenInteractive::Fullscreen();
    
    bool submitted = false;
    bool cancelled = false;

    std::vector<std::string> radiobox_entries = {
        "Search by Title",
        "Search by Category",
        "Search by Author/Editor"
    };

    auto radiobox = Radiobox(&radiobox_entries, &criteria_index);
    
    Component input_term = Input(&search_term, "Type search keyword...");

    auto submit_btn = Button("  Search  ", [&] { submitted = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto cancel_btn = Button("  Cancel  ", [&] { cancelled = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    // Group them to make them navigable
    auto container = Container::Vertical({
        radiobox,
        input_term,
        Container::Horizontal({submit_btn, cancel_btn})
    });

    // Render the layout
    auto renderer = Renderer(container, [&] {
        Elements layout;

        layout.push_back(separatorEmpty());
        layout.push_back(text(" SEARCH FOR A RESOURCE ") | bold | color(Color::CyanLight) | center);
        layout.push_back(separatorLight() | color(Color::GrayDark));
        layout.push_back(separatorEmpty());

        // Inject anything sitting in the message buffer
        if (!message_buffer.empty()) {
            Elements text_elements;
            std::istringstream stream(message_buffer);
            std::string line;

            while (std::getline(stream, line)) {
                if (line.empty() || line == "\r") text_elements.push_back(separatorEmpty());
                else text_elements.push_back(text(line)); 
            }

            layout.push_back(vbox(text_elements) | center);
            layout.push_back(separatorEmpty());
            layout.push_back(separatorLight() | color(Color::GrayDark));
            layout.push_back(separatorEmpty());
        }

        layout.push_back(hbox(text(" Criteria : ") | color(Color::White), radiobox->Render() | flex) | borderRounded);
        layout.push_back(hbox(text(" Keyword  : ") | color(Color::White), input_term->Render() | flex) | borderRounded);
        
        layout.push_back(separatorEmpty());
        layout.push_back(hbox({text("") | flex, submit_btn->Render(), text("   "), cancel_btn->Render(), text("") | flex}));
        layout.push_back(separatorEmpty());

        return vbox(layout) | borderRounded | color(Color::BlueLight) | size(WIDTH, GREATER_THAN, 55) | center;
    });

    screen.Loop(renderer);
    
    message_buffer.clear(); // Clean up the buffer after showing the form

    return submitted;
}


/**************************************************************************************/


// Prints options for broadcasting a notification
int UserInterface::show_notification_options()
{
    std::vector<std::string> entries = {"Send to a specific member", "Broadcast to all members", "Back to menu"};
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? "> " : "  ";
        std::string suffix = state.active ? " <" : "  ";
        Element e = text(prefix + state.label + suffix) | center;
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);
    screen.Loop(build_menu_renderer(menu, " NOTIFICATION OPTIONS "));
    message_buffer.clear();

    return selected + 1;
}


/**************************************************************************************/


// Print options for modifying profile
int UserInterface::show_profile_options()
{
    std::vector<std::string> entries = {"Edit your profile", "Close your account", "Back to menu"};
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? "> " : "  ";
        std::string suffix = state.active ? " <" : "  ";
        Element e = text(prefix + state.label + suffix) | center;
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);
    screen.Loop(build_menu_renderer(menu, " PROFILE OPTIONS "));
    message_buffer.clear();

    return selected + 1;
}


/**************************************************************************************/


// push these details into our message buffer for printing later
void UserInterface::show_membership_perks()
{
    add_text_to_buffer("\nStandard Membership:\n");
    add_text_to_buffer("-> Can borrow upto 2 resources at once\n");
    add_text_to_buffer("-> Can perform upto 2 transactions in a day\n");
    add_text_to_buffer("-> Can borrow a resource for 1 week\n");

    add_text_to_buffer("\nPremium Membership:\n");
    add_text_to_buffer("-> Can borrow upto 5 resources at once\n");
    add_text_to_buffer("-> Can perform upto 5 transactions in a day\n");
    add_text_to_buffer("-> Can borrow a resource for 2 weeks\n");

    add_text_to_buffer("\nHow To Upgrade To Premium?\n");
    add_text_to_buffer("-> Donate atleast 3 resources to our library\n");
    add_text_to_buffer("-> Unlock premium membership for a one-time fee of Rs.2000\n");
}


/**************************************************************************************/


// Print options for upgrading membership
int UserInterface::show_membership_upgrade_options()
{
    std::vector<std::string> entries = {"Donate a Resource", "Pay a one-time fee", "Back to menu"};
    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    
    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? "> " : "  ";
        std::string suffix = state.active ? " <" : "  ";
        Element e = text(prefix + state.label + suffix) | center;
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::GrayLight);
    };

    auto menu = Menu(&entries, &selected, option);
    screen.Loop(build_menu_renderer(menu, " UPGRADE MEMBERSHIP "));
    message_buffer.clear();

    return selected + 1;
}


/**************************************************************************************/


// Appends text to the buffer instead of printing immediately
void UserInterface::add_text_to_buffer(const std::string& prompt)
{
    message_buffer += prompt;
}


/**************************************************************************************/


// Acts as the renderer for all accumulated messages. Shows a modal and waits for input.
void UserInterface::show_buffered_message()
{
    // If there is no message to show, just return safely
    if (message_buffer.empty()) return; 

    auto screen = ScreenInteractive::Fullscreen();
    
    // Create an interactive button that the user can press to acknowledge
    auto btn = Button("  OK  ", screen.ExitLoopClosure(), ButtonOption::Animated());

    auto renderer = Renderer(btn, [&] {
        Elements text_elements;
        std::istringstream stream(message_buffer);
        std::string line;
        
        while (std::getline(stream, line)) {
            if (line.empty() || line == "\r") {
                text_elements.push_back(separatorEmpty());
            } else {
                text_elements.push_back(text(line)); 
            }
        }

        return vbox({
            separatorEmpty(),
            text(" SYSTEM MESSAGE ") | bold | color(Color::YellowLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            vbox(text_elements) | center,
            
            separatorEmpty(),
            btn->Render() | center,
            separatorEmpty(),
        }) 
        | borderRounded 
        | color(Color::CyanLight) 
        | size(WIDTH, GREATER_THAN, 50) 
        | center;
    });

    screen.Loop(renderer);

    message_buffer.clear();
}


/**************************************************************************************/


// Renders an interactive, scrollable list of items and returns the 1-based index of the selection.
// Returns 0 if the user clicks the "Go Back" button.
int UserInterface::show_selection_list(const std::string& title, const std::vector<std::string>& items)
{
    if (items.empty()) return 0;

    int selected = 0;
    bool cancelled = false;

    auto screen = ScreenInteractive::Fullscreen();

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    
    option.entries_option.transform = [](const EntryState& state) {
        std::string prefix = state.active ? "> " : "  ";
        Element e = text(prefix + state.label);
        if (state.active) return e | bold | color(Color::CyanLight) | inverted;
        return e | color(Color::White);
    };

    auto menu = Menu(&items, &selected, option);

    auto btn_cancel = Button(" Cancel / Go Back ", [&] { 
        cancelled = true; 
        screen.ExitLoopClosure()(); 
    }, ButtonOption::Animated());

    auto container = Container::Vertical({
        menu,
        btn_cancel
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            separatorEmpty(),
            text(" " + title + " ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            menu->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 15),
            
            separatorEmpty(),
            text("Use arrows to navigate. Press ENTER to select.") | dim | center,
            separatorEmpty(),
            btn_cancel->Render() | center,
            separatorEmpty()
        }) 
        | borderRounded 
        | color(Color::BlueLight) 
        | size(WIDTH, GREATER_THAN, 60) 
        | center;
    });

    screen.Loop(renderer);

    if (cancelled) return 0;
    return selected + 1;
}


/**************************************************************************************/


// Renders a multi-input form for new member registration.
// Returns true if the user clicks Submit, or false if they Cancel.
bool UserInterface::show_registration_form(std::string& fullname, std::string& username, std::string& password, std::string& address, std::string& deposit, const std::string& error_msg)
{
    auto screen = ScreenInteractive::Fullscreen();
    
    bool submitted = false;
    bool cancelled = false;

    // Mask the password field with asterisks
    InputOption password_option;
    password_option.password = true;

    // Create the input components
    Component input_fullname = Input(&fullname, "Type full name...");
    Component input_username = Input(&username, "Type username [a-z, 0-9]...");
    Component input_password = Input(&password, "Type strong password...", password_option);
    Component input_address  = Input(&address, "Type residential address...");
    Component input_deposit  = Input(&deposit, "Type initial deposit (500 - 5000)...");

    // Create the buttons
    auto submit_btn = Button("  Register  ", [&] { submitted = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto cancel_btn = Button("  Cancel  ", [&] { cancelled = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    // Group them into a navigable vertical container
    auto container = Container::Vertical({
        input_fullname,
        input_username,
        input_password,
        input_address,
        input_deposit,
        Container::Horizontal({submit_btn, cancel_btn})
    });

    // Render the form layout
    auto renderer = Renderer(container, [&] {
        return vbox({
            separatorEmpty(),
            text(" NEW MEMBER REGISTRATION ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            hbox(text(" Full Name    : ") | color(Color::White), input_fullname->Render() | flex) | borderRounded,
            hbox(text(" Username     : ") | color(Color::White), input_username->Render() | flex) | borderRounded,
            hbox(text(" Password     : ") | color(Color::White), input_password->Render() | flex) | borderRounded,
            hbox(text(" Address      : ") | color(Color::White), input_address->Render() | flex) | borderRounded,
            hbox(text(" Deposit (Rs) : ") | color(Color::White), input_deposit->Render() | flex) | borderRounded,
            
            separatorEmpty(),

            (error_msg.empty() ? text("") : text(error_msg) | color(Color::RedLight) | blink | center),

            separatorEmpty(),
            
            // Center the buttons at the bottom
            hbox({
                text("") | flex,
                submit_btn->Render(),
                text("   "),
                cancel_btn->Render(),
                text("") | flex
            }),

            separatorEmpty()
        }) 
        | borderRounded 
        | color(Color::BlueLight) 
        | size(WIDTH, GREATER_THAN, 60) 
        | center;
    });

    screen.Loop(renderer);

    return submitted;
}


/**************************************************************************************/


// Renders a multi-input form for user login.
// Returns true if the user clicks Submit, or false if they Cancel.
bool UserInterface::show_login_form(std::string& username, std::string& password, const std::string& error_msg)
{
    auto screen = ScreenInteractive::Fullscreen();

    bool submitted = false;
    bool cancelled = false;

    InputOption password_option;
    password_option.password = true;

    Component input_username = Input(&username, "Type username...");
    Component input_password = Input(&password, "Type password...", password_option);

    auto submit_btn = Button("  Login  ", [&] { submitted = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto cancel_btn = Button("  Cancel  ", [&] { cancelled = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    auto container = Container::Vertical({
        input_username,
        input_password,
        Container::Horizontal({submit_btn, cancel_btn})
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            separatorEmpty(),
            text(" ACCOUNT LOGIN ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            hbox(text(" Username : ") | color(Color::White), input_username->Render() | flex) | borderRounded,
            hbox(text(" Password : ") | color(Color::White), input_password->Render() | flex) | borderRounded,
            
            separatorEmpty(),

            (error_msg.empty() ? text("") : text(error_msg) | color(Color::RedLight) | blink | center),

            separatorEmpty(),
            
            hbox({
                text("") | flex,
                submit_btn->Render(), text("   "), cancel_btn->Render(),
                text("") | flex
            }),

            separatorEmpty()
        }) 
        | borderRounded | color(Color::BlueLight) | size(WIDTH, GREATER_THAN, 50) | center;
    });

    screen.Loop(renderer);
    return submitted;
}


/**************************************************************************************/


// Renders a multi-input form for updating member profile.
bool UserInterface::show_profile_form(std::string& fullname, std::string& username, std::string& password, std::string& address, const std::string& error_msg)
{
    auto screen = ScreenInteractive::Fullscreen();

    bool submitted = false;
    bool cancelled = false;

    InputOption password_option;
    password_option.password = true;

    Component input_fullname = Input(&fullname, "Full name...");
    Component input_username = Input(&username, "Username...");
    Component input_password = Input(&password, "New password...", password_option);
    Component input_address  = Input(&address, "Detailed address...");

    auto submit_btn = Button("  Save Changes  ", [&] { submitted = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto cancel_btn = Button("  Cancel  ", [&] { cancelled = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    auto container = Container::Vertical({
        input_fullname,
        input_username,
        input_password,
        input_address,
        Container::Horizontal({submit_btn, cancel_btn})
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            separatorEmpty(),
            text(" EDIT PROFILE ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            hbox(text(" Full Name : ") | color(Color::White), input_fullname->Render() | flex) | borderRounded,
            hbox(text(" Username  : ") | color(Color::White), input_username->Render() | flex) | borderRounded,
            hbox(text(" Password  : ") | color(Color::White), input_password->Render() | flex) | borderRounded,
            hbox(text(" Address   : ") | color(Color::White), input_address->Render() | flex) | borderRounded,
            
            separatorEmpty(),
            (error_msg.empty() ? text("") : text(error_msg) | color(Color::RedLight) | blink | center),
            separatorEmpty(),
            
            hbox({
                text("") | flex,
                submit_btn->Render(), text("   "), cancel_btn->Render(),
                text("") | flex
            }),
            separatorEmpty()
        }) 
        | borderRounded | color(Color::BlueLight) | size(WIDTH, GREATER_THAN, 60) | center;
    });

    screen.Loop(renderer);
    return submitted;
}


/**************************************************************************************/


// Renders a dynamic multi-input form for adding/updating resources based on their type.
bool UserInterface::show_resource_form(int type, std::string& title, std::string& category, std::string& pub_date, std::string& fine, std::vector<std::string>& extra_vals, const std::string& error_msg)
{
    auto screen = ScreenInteractive::Fullscreen();

    bool submitted = false;
    bool cancelled = false;

    // Define the specific labels depending on the resource type
    std::vector<std::string> extra_labels;
    if (type == 1) extra_labels = {"Journal Name", "Author Name", "Issue Number"};
    else if (type == 2) extra_labels = {"ISBN", "Author Name", "Publisher", "Edition", "Language"};
    else if (type == 3) extra_labels = {"ISSN", "Editor Name", "Publisher", "Volume No."};

    // Create Core Components
    Component input_title = Input(&title, "Title...");
    Component input_cat   = Input(&category, "Category...");
    Component input_date  = Input(&pub_date, "DD/MM/YYYY...");
    Component input_fine  = Input(&fine, "Fine per day...");

    // Dynamically Create Extra Components
    std::vector<Component> extra_inputs;
    for (int i = 0; i < extra_labels.size(); ++i) {
        extra_inputs.push_back(Input(&extra_vals[i], "Type here..."));
    }

    auto submit_btn = Button("  OK  ", [&] { submitted = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto cancel_btn = Button("  Cancel  ", [&] { cancelled = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    // Group everything into a single navigable container
    Components all_components = {input_title, input_cat, input_date, input_fine};
    all_components.insert(all_components.end(), extra_inputs.begin(), extra_inputs.end());
    all_components.push_back(Container::Horizontal({submit_btn, cancel_btn}));
    auto container = Container::Vertical(all_components);

    // Formatter to keep labels perfectly aligned
    auto format_label = [](const std::string& text_val) {
        std::string padded = " " + text_val + " ";
        int target_length = 22;
        
        // Safety check to prevent negative length values
        if (padded.length() < target_length) {
            padded.append(target_length - padded.length(), ' '); 
        }
        return padded;
    };

    auto renderer = Renderer(container, [&] {
        Elements layout;

        layout.push_back(separatorEmpty());
        layout.push_back(text(type == 1 ? " ARTICLE DETAILS " : (type == 2 ? " BOOK DETAILS " : " JOURNAL DETAILS ")) | bold | color(Color::CyanLight) | center);
        layout.push_back(separatorLight() | color(Color::GrayDark));
        layout.push_back(separatorEmpty());

        // Render Core Fields
        layout.push_back(hbox(text(format_label("Title")) | color(Color::White), input_title->Render() | flex) | borderRounded);
        layout.push_back(hbox(text(format_label("Category")) | color(Color::White), input_cat->Render() | flex) | borderRounded);
        layout.push_back(hbox(text(format_label("Publication Date")) | color(Color::White), input_date->Render() | flex) | borderRounded);
        layout.push_back(hbox(text(format_label("Fine per day (Rs)")) | color(Color::White), input_fine->Render() | flex) | borderRounded);

        // Render Dynamic Fields
        for (int i = 0; i < extra_labels.size(); ++i) {
            layout.push_back(hbox(text(format_label(extra_labels[i])) | color(Color::White), extra_inputs[i]->Render() | flex) | borderRounded);
        }

        layout.push_back(separatorEmpty());

        if (!error_msg.empty()) {
            layout.push_back(text(error_msg) | color(Color::RedLight) | blink | center);
            layout.push_back(separatorEmpty());
        }

        layout.push_back(hbox({text("") | flex, submit_btn->Render(), text("   "), cancel_btn->Render(), text("") | flex}));
        layout.push_back(separatorEmpty());

        return vbox(layout) | borderRounded | color(Color::Magenta) | size(WIDTH, GREATER_THAN, 60) | center;
    });

    screen.Loop(renderer);
    return submitted;
}


/**************************************************************************************/


// Renders a multi-input form for sending notifications.
// Dynamically adds a "Target Username" field if messaging to a specific member
bool UserInterface::show_notification_form(int choice, std::string& title, std::string& message, std::string& resource_id, std::string& target_username, const std::string& error_msg)
{
    auto screen = ScreenInteractive::Fullscreen();

    bool submitted = false;
    bool cancelled = false;

    // Create the Input Components
    Component input_title = Input(&title, "Title...");
    Component input_msg   = Input(&message, "Message...");
    Component input_res   = Input(&resource_id, "Resource ID (or -1 to skip)...");
    Component input_user  = Input(&target_username, "Member's username...");

    auto submit_btn = Button("  Send Notification  ", [&] { submitted = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto cancel_btn = Button("  Cancel  ", [&] { cancelled = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    // Build the navigation container dynamically
    Components all_components = {input_title, input_msg, input_res};
    if (choice == 1) {
        all_components.push_back(input_user); // Only add if sending a Direct Message
    }
    all_components.push_back(Container::Horizontal({submit_btn, cancel_btn}));
    
    auto container = Container::Vertical(all_components);

    // Render the UI
    auto renderer = Renderer(container, [&] {
        Elements layout;
        layout.push_back(separatorEmpty());
        layout.push_back(text(choice == 1 ? " DIRECT MESSAGE " : " BROADCAST MESSAGE ") | bold | color(Color::CyanLight) | center);
        layout.push_back(separatorLight() | color(Color::GrayDark));
        layout.push_back(separatorEmpty());

        layout.push_back(hbox(text(" Title       : ") | color(Color::White), input_title->Render() | flex) | borderRounded);
        layout.push_back(hbox(text(" Message     : ") | color(Color::White), input_msg->Render() | flex) | borderRounded);
        layout.push_back(hbox(text(" Resource ID : ") | color(Color::White), input_res->Render() | flex) | borderRounded);

        // Render the extra username input box if choice is 1
        if (choice == 1) {
            layout.push_back(hbox(text(" Username    : ") | color(Color::White), input_user->Render() | flex) | borderRounded);
        }

        layout.push_back(separatorEmpty());

        if (!error_msg.empty()) {
            layout.push_back(text(error_msg) | color(Color::RedLight) | blink | center);
            layout.push_back(separatorEmpty());
        }

        layout.push_back(hbox({text("") | flex, submit_btn->Render(), text("   "), cancel_btn->Render(), text("") | flex}));
        layout.push_back(separatorEmpty());

        return vbox(layout) | borderRounded | color(Color::Magenta) | size(WIDTH, GREATER_THAN, 60) | center;
    });

    screen.Loop(renderer);
    return submitted;
}


/**************************************************************************************/


// Renders an array of text as a scrollable view using a hidden menu.
void UserInterface::show_report_view(const std::string& title, const std::vector<std::string>& lines)
{
    auto screen = ScreenInteractive::Fullscreen();
    int selected = 0;

    // Configure the menu to act purely as a scrollable text view
    MenuOption option;
    option.on_enter = screen.ExitLoopClosure(); // Pressing enter anywhere exits

    option.entries_option.transform = [](const EntryState& state) {
        return text(" " + state.label + " ") | color(Color::White); 
    };

    auto menu = Menu(&lines, &selected, option);
    auto btn_back = Button("  Go Back  ", screen.ExitLoopClosure(), ButtonOption::Animated());

    auto container = Container::Vertical({
        menu,
        btn_back
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            separatorEmpty(),
            text(" " + title + " ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            menu->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 20) | center,
            
            separatorEmpty(),
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            text("Press ENTER to exit") | dim | center,
            separatorEmpty(),
            btn_back->Render() | center,
            separatorEmpty()
        }) 
        | borderRounded 
        | color(Color::YellowLight) 
        | size(WIDTH, GREATER_THAN, 85)
        | center;
    });

    screen.Loop(renderer);
}


/**************************************************************************************/


// Renders a confirmation dialog with Yes/No buttons. 
// Also injects any buffered messages above the prompt
bool UserInterface::get_confirmation(const std::string& prompt)
{
    auto screen = ScreenInteractive::Fullscreen();
    bool result = false;

    // Interactive buttons
    auto btn_yes = Button("   Yes   ", [&] { result = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto btn_no  = Button("   No    ", [&] { result = false; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    auto container = Container::Horizontal({btn_yes, btn_no});

    auto renderer = Renderer(container, [&] {
        Elements layout;

        layout.push_back(separatorEmpty());
        layout.push_back(text(" SYSTEM CONFIRMATION ") | bold | color(Color::YellowLight) | center);
        layout.push_back(separatorLight() | color(Color::GrayDark));
        layout.push_back(separatorEmpty());

        // Inject anything sitting in the message buffer
        if (!message_buffer.empty()) {
            Elements text_elements;
            std::istringstream stream(message_buffer);
            std::string line;
            while (std::getline(stream, line)) {
                if (line.empty() || line == "\r") text_elements.push_back(separatorEmpty());
                else text_elements.push_back(text(line)); 
            }
            layout.push_back(vbox(text_elements) | center);
            layout.push_back(separatorEmpty());
            layout.push_back(separatorLight() | color(Color::GrayDark));
            layout.push_back(separatorEmpty());
        }

        // The prompt and buttons
        layout.push_back(text(prompt) | bold | color(Color::White) | center);
        layout.push_back(separatorEmpty());
        layout.push_back(hbox({text("") | flex, btn_yes->Render(), text("   "), btn_no->Render(), text("") | flex}));
        layout.push_back(separatorEmpty());

        return vbox(layout) | borderRounded | color(Color::CyanLight) | size(WIDTH, GREATER_THAN, 50) | center;
    });

    screen.Loop(renderer);
    message_buffer.clear(); // Clear the buffer after consuming it

    return result;
}


/**************************************************************************************/


// Renders an input form for depositing funds.
// Returns true if the user clicks Deposit, or false if they Cancel.
bool UserInterface::show_deposit_form(int current_balance, std::string& deposit_amount, const std::string& error_msg)
{
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    bool submitted = false;
    bool cancelled = false;

    Component input_amount = Input(&deposit_amount, "Enter amount (Rs)...");

    auto submit_btn = Button("  Deposit  ", [&] { submitted = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());
    auto cancel_btn = Button("  Cancel  ", [&] { cancelled = true; screen.ExitLoopClosure()(); }, ButtonOption::Animated());

    auto container = Container::Vertical({
        input_amount,
        Container::Horizontal({submit_btn, cancel_btn})
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            separatorEmpty(),
            text(" DEPOSIT FUNDS ") | bold | color(Color::CyanLight) | center,
            separatorLight() | color(Color::GrayDark),
            separatorEmpty(),
            
            // Display the users current balance for context
            text("Current Balance: Rs." + std::to_string(current_balance)) | bold | color(Color::YellowLight) | center,
            separatorEmpty(),

            hbox(text(" Amount : ") | color(Color::White), input_amount->Render() | flex) | borderRounded,
            
            separatorEmpty(),
            (error_msg.empty() ? text("") : text(error_msg) | color(Color::RedLight) | blink | center),
            separatorEmpty(),
            
            hbox({
                text("") | flex,
                submit_btn->Render(), text("   "), cancel_btn->Render(),
                text("") | flex
            }),
            separatorEmpty()
        }) 
        | borderRounded | color(Color::GreenLight) | size(WIDTH, GREATER_THAN, 50) | center;
    });

    screen.Loop(renderer);
    return submitted;
}


/**************************************************************************************/