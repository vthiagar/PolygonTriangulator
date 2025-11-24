import sys
import csv
import matplotlib.pyplot as plt


def load_polygon(csv_path):
    xs, ys = [], []
    with open(csv_path, newline="") as f:
        reader = csv.reader(f)
        for row in reader:
            if not row:
                continue
            x = float(row[0])
            y = float(row[1])
            xs.append(x)
            ys.append(y)
    return xs, ys


def load_triangles_indices(csv_path):
    tris = []
    with open(csv_path, newline="") as f:
        reader = csv.reader(f)
        for row in reader:
            if not row:
                continue
            i = int(row[0])
            j = int(row[1])
            k = int(row[2])
            tris.append((i, j, k))
    return tris


def main():
    if len(sys.argv) < 3:
        print("Usage: python ../../PolygonTriangulator/visualize_triangulation.py <polygon_csv> <triangles_csv>")
        sys.exit(1)

    poly_csv = sys.argv[1]
    tri_csv = sys.argv[2]

    xs, ys = load_polygon(poly_csv)
    tris = load_triangles_indices(tri_csv)

    fig, ax = plt.subplots(figsize=(6, 6))

    # --- Draw polygon in red (closed loop) ---
    if xs and ys:
        xs_closed = xs + [xs[0]]
        ys_closed = ys + [ys[0]]
        ax.plot(xs_closed, ys_closed, color="red", linewidth=2, label="Polygon")

    # --- Draw each triangle as a filled patch in blue ---
    n = len(xs)
    for (i, j, k) in tris:
        if not (0 <= i < n and 0 <= j < n and 0 <= k < n):
            continue

        tx = [xs[i], xs[j], xs[k]]
        ty = [ys[i], ys[j], ys[k]]

        # Fill so you can *see* the triangle, not just its edges
        ax.fill(tx, ty, facecolor="blue", edgecolor="black", alpha=0.3, linewidth=1)

    ax.set_aspect("equal", adjustable="box")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.legend()
    ax.set_title("Polygon and Ear-Clipping Triangulation")

    plt.show()


if __name__ == "__main__":
    main()
