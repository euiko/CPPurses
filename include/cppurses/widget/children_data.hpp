#ifndef CPPURSES_WIDGET_CHILDREN_DATA_HPP
#define CPPURSES_WIDGET_CHILDREN_DATA_HPP
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cppurses {
class Widget;

/// Contains all data relevant to child Widgets for the Widget class.
class Children_data {
   public:
    /// Must pass in the parent so that newly added Widgets can know about them.
    Children_data(Widget* parent);

    /// Add a Widget as a child to the owning Widget. \p child is inserted at
    /// the end of the list of children.
    void add(std::unique_ptr<Widget> child);

    /// Insert a Widget as a child into the owning Widget at a specified index.
    /// Contains bound checking for index, will no-op if out of range.
    void insert(std::unique_ptr<Widget> child, std::size_t index);

    /// Checks if the owning Widget has the child pointed to by the provided
    /// pointer.
    bool has(Widget* child) const;

    /// Checks if the owning Widget has the child with \p name.
    bool has(const std::string& name) const;

    /// Checks if the owning Widget has the descendant pointed to by the
    /// provided pointer.
    bool has_descendant(Widget* descendant) const;

    /// Checks if the owning Widget has the descendent with \p name.
    bool has_descendant(const std::string& name) const;

    /// Returns a reference to the list of children.
    const std::vector<std::unique_ptr<Widget>>& get() const;

    /// Returns a list of all descendants in breadth first order. Descendants
    /// are children, or children of children to any level down the tree.
    std::vector<Widget*> get_descendants() const;

    /// Remove a child from the list, by pointer value, and return it. Useful if
    /// you need to move a child widget into another widget. Use Widget::close()
    /// if you want to remove a widget for good. Letting the returned Widget
    /// destroy itself will potentially leave dangling pointers in the event
    /// system.
    std::unique_ptr<Widget> remove(Widget* child);

    /// Remove a child from the list, by name, and return it.
    std::unique_ptr<Widget> remove(const std::string& name);

   private:
    friend class Widget;
    Widget* parent_;
    std::vector<std::unique_ptr<Widget>> children_;
};

}  // namespace cppurses
#endif  // CPPURSES_WIDGET_CHILDREN_DATA_HPP
