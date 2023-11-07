cmake_minimum_required(VERSION "3.26.4")

project("app2")

add_executable(
  ${PROJECT_NAME}
  "app2.c"
)

target_link_libraries(app2 "-L/nix/store/pjk6gld9s6434ykb76cmqpgxbqw8sf4b-mariadb-connector-c-3.3.5/lib/mariadb" "mariadb")
