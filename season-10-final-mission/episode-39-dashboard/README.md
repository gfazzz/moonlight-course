# Episode 39: "Intelligence Dashboard" 📊
## Season 10: FINAL MISSION | Episode 39/42

> *"See everything. Know everything."*

---

## 📋 Briefing

Создать real-time TUI dashboard для всей системы MOONLIGHT.

**Задачи:**
1. Multi-panel TUI (ncurses)
2. Real-time data feeds
3. Network visualization
4. Alert system

---

## 📚 Теория

### Advanced ncurses

```c
WINDOW *create_panel(int h, int w, int y, int x, const char *title) {
    WINDOW *win = newwin(h, w, y, x);
    box(win, 0, 0);
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 2, " %s ", title);
    wattroff(win, A_BOLD);
    wrefresh(win);
    return win;
}

void update_status(WINDOW *win, const char *status) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Status: %s", status);
    wrefresh(win);
}
```

---

## 🛠 Практика

**Задачи:**
1. Network map display
2. Live log viewer
3. System metrics
4. Threat visualization

---

**Next:** [Episode 40: Offensive & Defensive →](../episode-40-offensive-defensive/)
