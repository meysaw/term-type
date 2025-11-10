#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>
#include <atomic>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>
#include <vector>
using namespace ftxui;
std::string gettext();
int main()
{
  std::string target_text = gettext();
  std::replace(target_text.begin(), target_text.end(), '\n', ' ');
  std::string typed_text = "";
  int right = 0;
  int typed_length = 0;
  std::atomic<int> time_left = 30;
  bool running = true;

  auto screen = ScreenInteractive::FullscreenAlternateScreen();
  std::thread timer_thread;

  std::function<void()> start_timer;
  std::function<void()> reset_test;

  start_timer = [&]()
  {
    timer_thread = std::thread([&]
                               {
          while (time_left > 0 && running) {
              std::this_thread::sleep_for(std::chrono::seconds(1));
              if (!running) {
                  break; 
              }
              time_left--;
              screen.PostEvent(Event::Custom);
          }
          if (time_left == 0 && running) {
              running = false;
              screen.PostEvent(Event::Custom);
          } });
  };

  reset_test = [&]()
  {
    running = false;
    if (timer_thread.joinable())
    {
      timer_thread.join();
    }
    target_text = gettext();
    std::replace(target_text.begin(), target_text.end(), '\n', ' ');
    typed_text = "";
    right = 0;
    typed_length = 0;
    time_left = 30;
    running = true;
    start_timer();
  };
  reset_test();

  auto renderer = Renderer([&]
                           {

    Elements chars;
    for (size_t i = 0; i < target_text.size(); ++i) {
     
      Element char_element; 
      if (i < typed_text.size()) {
        if (typed_text[i] == target_text[i])
         { char_element = text(std::string(1, target_text[i])) | color(Color::Green)| bold;
        }
        else
          char_element = text(std::string(1, target_text[i])) | color(Color::Red ) | bold;
      } else {
        char_element = text(std::string(1, target_text[i])) | dim ;
      }

      
      if (i == typed_text.size()) {
          char_element = char_element | underlined | inverted;
      }
      chars.push_back(std::move(char_element));
    }

auto sentence = flexbox(std::move(chars), FlexboxConfig().Set(FlexboxConfig::Direction::Row).Set(FlexboxConfig::Wrap::Wrap)) | center | border | size(HEIGHT, GREATER_THAN, 5);
    return vbox({
      text(" TERM_TYPE") | bold | center| color(Color::DarkOliveGreen1),
      text("Time left: " + std::to_string(time_left.load()) + " sec") | center| color(Color::DarkOliveGreen1),
      separator() | color(Color::DarkOliveGreen1),
      sentence   ,
      separator() | color(Color::DarkOliveGreen1),
      text("Type faster lol..") | center | color(Color::DarkOliveGreen1)
    }); });
  auto ls = Renderer([&]
                     { int elapsed = 30 - time_left.load();
                        if(elapsed <= 0) elapsed = 1;
                        double correct_wpm = (static_cast<double>(right) / 5.0) * (60.0 / elapsed);
                        int accuracy = (typed_length > 0) ? (right * 100 / typed_length) : 100;
    double raw_wpm = (static_cast<double>(typed_length) / 5.0) * (60.0 / elapsed);
    Element stats = vbox({
        text("Accuracy: " + std::to_string(accuracy) + "%") | bold | center,
        text("Your wpm was: " + std::to_string(static_cast<int>(std::round(correct_wpm)))) | bold | center,
        text("Your raw was: " + std::to_string(static_cast<int>(std::round(raw_wpm)))) | center | bold
    }) | border;
    return vbox({text("Your stats are" ) | center | color(Color::DarkOliveGreen1),separator()|color(Color::DarkOliveGreen1),stats , separator()|color(Color::DarkOliveGreen1),text("Press 'R' to restart or 'Esc' to quit.") | center | color(Color::DarkOliveGreen1)}); });
  text("Press 'R' 'Esc' to quit.") | center | color(Color::DarkOliveGreen1);
  auto main_ui = Renderer([&]
                          {
  if (!running)
    return ls->Render();
  else
    return renderer->Render(); });
  auto component = CatchEvent(main_ui, [&](Event event)
                              {
    if (event == Event::Escape) {
      running = false;
      screen.ExitLoopClosure()();
      return true;
    }

   if (!running) {
        if (event == Event::Character('r') || event == Event::Character('R') || event == Event::Return) {
            
            reset_test();
            return true;
        }
        return false; 
      }

    if (event.is_character()) {
      if (typed_text.size() < target_text.size()) {
        typed_text += event.character();
    typed_length++;
    if (event.character()[0] == target_text[typed_text.size() - 1])
      right++;
      if (typed_text.size() == target_text.size()) {
    running = false;
    screen.PostEvent(Event::Custom);  
    return true;
}

      }
      return true;
    }

    if (event == Event::Backspace && !typed_text.empty()) {
     size_t pos = typed_text.size() - 1;  
    char last = typed_text[pos];  
    if (last == target_text[pos]) {
        right--;
    }
    typed_text.pop_back();
    typed_length--;
    return true;
    }

    return false; });

  screen.Loop(component);

  running = false;
  if (timer_thread.joinable())
  {
    timer_thread.join();
  }

  return 0;
}
std::string gettext()
{
  std::vector<std::string> texts = {

      "Technology has reshaped every aspect of modern life, from how we communicate to how we work, learn, and even think. The smartphone, once a luxury, is now an extension of human identity, connecting billions through invisible waves of information. Artificial intelligence has begun to automate not just labor but creativity, composing music, writing code, and generating art. Yet this rapid progress also raises questions: Are we becoming too dependent on machines? As convenience increases, attention spans shrink, and privacy becomes fragile. The world is more connected than ever, but loneliness often grows behind screens. The challenge of the twenty-first century is not inventing smarter technology but ensuring it aligns with human values—ethics, empathy, and truth. The future will depend on our ability to balance innovation with wisdom, so that technology enhances humanity rather than replacing it.",

      "The art of cooking is a fascinating blend of science and creativity, transforming raw ingredients into complex, satisfying meals. From the precise measurements of baking to the intuitive seasoning of a stew, every step involves careful attention to chemical reactions and sensory balance. Culinary traditions around the world showcase incredible diversity, reflecting local resources and cultural history. Learning to cook is more than just following a recipe; it's about developing an understanding of flavors, textures, and aromas. It is a fundamental skill that connects us to our history and to each other, making the simple act of eating a deeply human experience.",

      "Astronomy is one of the oldest natural sciences, dealing with celestial objects, space, and the universe as a whole. Modern telescopes, both ground-based and orbiting in space, allow scientists to peer billions of light-years away, studying the formation of stars, galaxies, and planetary systems. The discoveries of dark matter and dark energy have shown that the vast majority of the universe remains a profound mystery. Space exploration, with missions to Mars and beyond, continues humanity's quest to understand its place in the cosmos. These explorations not only push the boundaries of technology but also inspire wonder and fundamental questions about life beyond Earth.",
      "Climate change is one of kjthe most pressing challenges facing humanity today. Driven primarily by the burning of fossil fuels and deforestation, it leads to rising global temperatures, melting ice caps, and more frequent extreme weather events. The consequences of climate change are far-reaching, affecting ecosystems, agriculture, and human health. Addressing this issue requires a coordinated global effort to reduce greenhouse gas emissions, transition to renewable energy sources, and implement sustainable practices. It also calls for adaptation strategies to protect vulnerable communities and preserve biodiversity. The fight against climate change is not just an environmental imperative but a moral one, demanding action from individuals, governments, and industries alike.",

      "Literature serves as a mirror to society, reflecting its values, struggles, and aspirations through the written word. From ancient epics to contemporary novels, literature captures the human experience in all its complexity. It allows readers to explore different cultures, historical periods, and philosophical ideas, fostering empathy and understanding. The power of storytelling lies in its ability to convey emotions and provoke thought, challenging readers to question their beliefs and perceptions. In an increasingly digital world, literature remains a vital medium for preserving language, culture, and the shared narratives that define humanity."};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, texts.size() - 1);

  return texts[distrib(gen)];
}