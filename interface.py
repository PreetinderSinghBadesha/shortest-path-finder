import tkinter as tk
from tkinter import messagebox
import subprocess
import os

nodes = []
edges = []
start = None
end = None

def add_node(event):
    global nodes
    x, y = event.x, event.y
    canvas.create_oval(x-5, y-5, x+5, y+5, fill="black")
    nodes.append((x, y))

def add_edge():
    if len(nodes) < 2:
        messagebox.showwarning("Warning", "You need at least two nodes to add an edge.")
        return
    global edges
    node1 = nodes[-2]
    node2 = nodes[-1]
    canvas.create_line(node1[0], node1[1], node2[0], node2[1], fill="blue")
    edges.append((node1, node2))

def set_start(event):
    global start
    start = (event.x, event.y)
    canvas.create_text(event.x, event.y, text="S", fill="green", font=("Arial", 14))

def set_end(event):
    global end
    end = (event.x, event.y)
    canvas.create_text(event.x, event.y, text="E", fill="red", font=("Arial", 14))

def calculate_shortest_path():
    if not start or not end:
        messagebox.showerror("Error", "Please set both start and end points.")
        return
    if not edges:
        messagebox.showerror("Error", "Please add some edges.")
        return
    
    with open("input.txt", "w") as f:
        f.write(f"{start[0]} {start[1]} {end[0]} {end[1]}\n")
        f.write(f"{len(nodes)}\n")
        for node in nodes:
            f.write(f"{node[0]} {node[1]}\n")
        f.write(f"{len(edges)}\n")
        for edge in edges:
            f.write(f"{edge[0][0]} {edge[0][1]} {edge[1][0]} {edge[1][1]}\n")
    
    try:
        subprocess.run(["shortest_path.exe"], check=True)
        with open("output.txt", "r") as f:
            path = f.readlines()
        
        points = [tuple(map(int, line.strip().split())) for line in path[1:]]
        for i in range(len(points) - 1):
            canvas.create_line(points[i][0], points[i][1], points[i+1][0], points[i+1][1], fill="green", width=2)
        messagebox.showinfo("Success", "Shortest path calculated and displayed.")
    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"C++ program failed: {e}")
    except Exception as e:
        messagebox.showerror("Error", f"An error occurred: {e}")

root = tk.Tk()
root.title("Shortest Path GUI")
canvas = tk.Canvas(root, width=800, height=600, bg="white")
canvas.pack()

canvas.bind("<Button-1>", add_node)
canvas.bind("<Button-3>", set_start)
canvas.bind("<Button-2>", set_end)

btn_edge = tk.Button(root, text="Add Edge", command=add_edge)
btn_edge.pack(side=tk.LEFT, padx=5, pady=5)

btn_calculate = tk.Button(root, text="Calculate Shortest Path", command=calculate_shortest_path)
btn_calculate.pack(side=tk.LEFT, padx=5, pady=5)

root.mainloop()