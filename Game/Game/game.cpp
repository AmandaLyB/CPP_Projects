#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_x = 0.f;
float player_y = 0.f;

internal void
simulate_game(Input* input, float dt) {
	clear_screen(0x4D8934);
    float speed = 50.f;
    if (is_down(BUTTON_UP)) player_y += speed*dt;
    if (is_down(BUTTON_DOWN)) player_y -= speed * dt;
    if (is_down(BUTTON_RIGHT)) player_x += speed * dt;
    if (is_down(BUTTON_LEFT)) player_x -= speed * dt;

    draw_rect(player_x, player_y, 2, 2, 0xF6B26B);
    draw_rect(-20, 20, 5, 5, 0xF6B26B);

}