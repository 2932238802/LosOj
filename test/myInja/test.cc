// #include <iostream>
// #include <nlohmann/json.hpp>

// #include "inja/inja.hpp"

// using json = nlohmann::json;

// int main()
// {
//     inja::Environment env;

//     json data;
//     data["user"]["name"] = "LosAngelous";
//     data["user"]["email"] = "los.a@example.com";
//     data["user"]["age"] = 30;
//     data["user"]["skills"] = {"C++", "CMake", "Linux", "vcpkg"};

//     try
//     {
//         std::string result = env.render_file("user_profile.tpl", data);
//         std::cout << "--- Rendered HTML ---" << std::endl;
//         std::cout << result << std::endl;
//     }
//     catch (const std::exception& e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }
