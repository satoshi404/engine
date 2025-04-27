import platform
import engine
import time

def main():
    try:
        # Create window
        config = platform.WindowConfig("Test Platform", 0, 0, 800, 600)

        window = platform_engine.Window(config)
        window.show()

        # Create renderer
        renderer = platform_engine.Renderer(window)

        # Create event handler
        event = platform_engine.Event(window)

        # Draw blue background
        blue = platform_engine.Color(0, 0, 100, 255)
        renderer.set_draw_color(blue.r, blue.g, blue.b, blue.a)
        renderer.draw_rect(0, 0, 800, 600, True, 1)

        # Red rectangle
        red = platform_engine.Color(255, 0, 0, 255)
        rect_id = 2
        rect_x = 200
        rect_velocity = 100.0  # Pixels/sec

        running = True
        last_time = time.time()

        # Main loop
        while running and window.should_run():
            # Handle events
            while event.poll():
                kind = event.kind()
                if kind == platform_engine.EventKind.KEY_ESC:
                    running = False
                elif kind == platform_engine.EventKind.KEY_SPACE:
                    rect_velocity = -rect_velocity  # Reverse direction

            # Update
            current_time = time.time()
            delta_time = current_time - last_time
            last_time = current_time

            # Update rectangle position
            renderer.remove_shape_by_id(rect_id)
            rect_x += rect_velocity * delta_time
            if rect_x <= 0 or rect_x + 50 >= 800:
                rect_velocity = -rect_velocity
                rect_x = max(0, min(rect_x, 750))
            renderer.set_draw_color(red.r, red.g, red.b, red.a)
            renderer.draw_rect(int(rect_x), 300, 50, 50, True, rect_id)

            # Render
            renderer.present()

            # Cap frame rate
            time.sleep(0.002)

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()