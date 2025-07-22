import openpyxl
from pptx import Presentation
from pptx.util import Inches, Pt
from pptx.enum.text import PP_ALIGN
from openpyxl.utils import column_index_from_string
import math

def get_dynamic_font_size(row_count):
    """
    Calculates a dynamic font size based on the number of rows on a slide.
    """
    if row_count <= 15:
        return Pt(12)  # Large font for few rows
    elif row_count <= 25:
        return Pt(10)  # Standard font for medium rows
    else:
        return Pt(8)   # Small font for many rows

def get_excel_data_and_merges(worksheet):
    """
    Reads data and merge information, creating maps for coordinate translation.
    """
    data = []
    hidden_rows = {r for r, dim in worksheet.row_dimensions.items() if dim.hidden}
    hidden_cols_indices = {column_index_from_string(c) for c, dim in worksheet.column_dimensions.items() if dim.hidden}

    row_map = {excel_row: ppt_row for ppt_row, excel_row in enumerate(
               r for r in range(1, worksheet.max_row + 1) if r not in hidden_rows)}
    col_map = {excel_col: ppt_col for ppt_col, excel_col in enumerate(
               c for c in range(1, worksheet.max_column + 1) if c not in hidden_cols_indices)}

    for excel_row_idx in sorted(row_map.keys()):
        row_data = [str(worksheet.cell(row=excel_row_idx, column=excel_col_idx).value or "")
                    for excel_col_idx in sorted(col_map.keys())]
        data.append(row_data)

    ppt_merges = []
    for merged_range in worksheet.merged_cells.ranges:
        min_col, min_row, max_col, max_row = merged_range.bounds
        if min_row in row_map and max_row in row_map and min_col in col_map and max_col in col_map:
            if not (row_map[min_row] == row_map[max_row] and col_map[min_col] == col_map[max_col]):
                ppt_merges.append({
                    "start_row": row_map[min_row], "start_col": col_map[min_col],
                    "end_row": row_map[max_row], "end_col": col_map[max_col]
                })

    return data, ppt_merges

def create_ppt_from_excel(excel_path, ppt_path):
    """
    Creates a PowerPoint presentation, handling pagination and dynamic font sizes.
    """
    try:
        workbook = openpyxl.load_workbook(excel_path, data_only=True)
        worksheet = workbook.active
    except FileNotFoundError:
        print(f"Error: The file '{excel_path}' was not found.")
        return

    table_data, ppt_merges = get_excel_data_and_merges(worksheet)
    if not table_data:
        print("No visible data found to convert.")
        return

    prs = Presentation()
    slide_layout = prs.slide_layouts[6]  # Blank layout
    
    header_row = table_data[0]
    data_rows = table_data[1:]
    num_cols = len(header_row)
    
    MAX_ROWS_PER_SLIDE = 20  # Set the maximum number of rows per slide (including header)
    
    data_chunks = [data_rows[i:i + MAX_ROWS_PER_SLIDE - 1] 
                   for i in range(0, len(data_rows), MAX_ROWS_PER_SLIDE - 1)]

    original_row_offset = 0
    for chunk in data_chunks:
        slide = prs.slides.add_slide(slide_layout)
        
        slide_table_data = [header_row] + chunk
        num_rows_for_slide = len(slide_table_data)
        font_size = get_dynamic_font_size(num_rows_for_slide)

        left, top, width, height = Inches(0.5), Inches(0.5), Inches(9.0), Inches(0.5 * num_rows_for_slide)
        shape = slide.shapes.add_table(num_rows_for_slide, num_cols, left, top, width, height)
        table = shape.table

        try:
            col_width = int(width / num_cols)
            for i in range(num_cols):
                table.columns[i].width = col_width
        except ZeroDivisionError: pass

        for r_idx, row_data in enumerate(slide_table_data):
            for c_idx, cell_data in enumerate(row_data):
                cell = table.cell(r_idx, c_idx)
                cell.text = cell_data
                p = cell.text_frame.paragraphs[0]
                p.font.size = font_size
                p.alignment = PP_ALIGN.LEFT
        
        # --- Handle merges for the current slide ---
        for merge in ppt_merges:
            # Check if the merge is relevant for the current chunk of data
            merge_start_in_chunk = merge["start_row"] - original_row_offset
            merge_end_in_chunk = merge["end_row"] - original_row_offset
            
            # The merge must start within the visible data rows of this slide (row 1 onwards)
            if 0 < merge_start_in_chunk < num_rows_for_slide:
                # Adjust coordinates to be relative to the slide's table
                final_start_row = merge_start_in_chunk
                final_end_row = min(merge_end_in_chunk, num_rows_for_slide - 1)
                
                start_cell = table.cell(final_start_row, merge["start_col"])
                end_cell = table.cell(final_end_row, merge["end_col"])
                start_cell.merge(end_cell)

        original_row_offset += len(chunk)

    prs.save(ppt_path)
    print(f"Presentation with {len(prs.slides)} slide(s) saved to '{ppt_path}'.")

def create_long_sample_excel(filename='SampleEdgeCaseExcel_Long.xlsx'):
    """
    Creates a long sample Excel file to test pagination.
    """
    wb = openpyxl.Workbook()
    ws = wb.active
    ws.title = "Pagination_Test"

    header = ["ID", "Product", "Category", "Sales", "Notes"]
    ws.append(header)
    
    for i in range(1, 41):
        ws.append([i, f"Product-{i:02d}", f"Category-{(i % 5) + 1}", 100 + i * 5, f"Note for product {i}"])

    # Add a merge that will span across a slide break
    ws.merge_cells('E25:E28')
    ws.cell(row=25, column=5).value = "This merge crosses a slide boundary"

    # Add a merge fully contained on the second slide
    ws.merge_cells('E35:E36')
    ws.cell(row=35, column=5).value = "Merge on second slide"

    wb.save(filename)
    print(f"Long sample Excel file '{filename}' created.")


# --- Execution ---
excel_filename = 'SampleEdgeCaseExcel_Long.xlsx'
ppt_filename = 'OutputPresentation_Paged.pptx'

# create_long_sample_excel(excel_filename)
create_ppt_from_excel(excel_filename, ppt_filename)
