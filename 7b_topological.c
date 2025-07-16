#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define x 4
#define y 20
int n;int c;
int count;
struct stack{
    int arr[20];
    int top;
};
typedef struct stack stack;
void push(stack *s,int data){
    s->arr[++s->top]=data;
}
int isempty(stack*s){
    if(s->top==-1){
        return 1;
    }
    else{
        return 0;
    }
}
int pop(stack* s){
        return s->arr[s->top--];
}
void dfs(int v,int n,int mat[][n],int vis[],stack * s){
    for(int i=0;i<n;i++){
        count++;
        if(mat[v][i]==1 && vis[i]==0){
            vis[i]=1;
            dfs(i,n,mat,vis,s);
        }
    }
    push(s,v);
}
void dfsmain(){
    FILE *a;FILE *b;
    a=fopen("output.txt","a");
    b=fopen("result.txt","a");
    int tm=0;
    srand(time(NULL));
    for(int j=2;j<20;j++){
        stack s;
        s.top=-1;
        n=j;tm=0;
        int arr[n][n];
        int isvis[n];
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                arr[i][j]=0;
            }
        }
            for(int i=1;i<n;i++){
                arr[i-1][i]=1;
            }
        int vis[n];
        for(int i=0;i<n;i++){
            vis[i]=0;
        }
            count=0;
        for(int i=0;i<n;i++){
            if(vis[i]==0){
                vis[i]=1;
                dfs(i,n,arr,vis,&s);
            }
        }
        fprintf(a,"%d\t%d\n",j,count);
        while(!isempty(&s)){
            int r=pop(&s);
            fprintf(b,"%d ",r);
        }
        fprintf(b,"\n");
    }
    fclose(a);
}
int main(){
    system("rm -r *.txt");
    dfsmain();
}



import os
from docx import Document
from pptx import Presentation
from pptx.util import Inches, Pt
from pptx.enum.text import PP_ALIGN
from io import BytesIO

# Constants
SLIDE_WIDTH = Inches(10)
SLIDE_HEIGHT = Inches(5.625)
LEFT_MARGIN = Inches(0.5)
TOP_MARGIN = Inches(0.5)
TABLE_WIDTH = SLIDE_WIDTH - 2 * LEFT_MARGIN
DEFAULT_FONT_NAME = 'Times New Roman'
DEFAULT_FONT_SIZE = Pt(14)
MAX_ROWS_PER_SLIDE = 6  # Split large tables for readability


def convert_docx_tables_to_pptx(docx_path: str, pptx_path: str):
    """
    Converts all tables from a DOCX file into a PPTX presentation.
    Merged cells are flattened. Images are placed near the corresponding table row.
    """
    try:
        doc = Document(docx_path)
    except Exception as e:
        print(f"Failed to load DOCX: {e}")
        return

    prs = Presentation()
    blank_slide = prs.slide_layouts[6]

    for table in doc.tables:
        if not any(any(cell.text.strip() for cell in row.cells) for row in table.rows):
            continue  # skip empty table

        flat_rows = []
        image_map = []
        max_cols = max(len(row.cells) for row in table.rows)

        for row in table.rows:
            texts = []
            images = []
            for cell in row.cells:
                text = '\n'.join(p.text.strip() for p in cell.paragraphs if p.text.strip())
                texts.append(text if text else "")
                images.append(extract_image_from_cell(cell))
            while len(texts) < max_cols:
                texts.append("")
                images.append(None)
            flat_rows.append(texts)
            image_map.append(images)

        # Split into chunks
        for i in range(0, len(flat_rows), MAX_ROWS_PER_SLIDE):
            chunk = flat_rows[i:i + MAX_ROWS_PER_SLIDE]
            img_chunk = image_map[i:i + MAX_ROWS_PER_SLIDE]
            slide = prs.slides.add_slide(blank_slide)

            rows = len(chunk)
            cols = max_cols

            table_shape = slide.shapes.add_table(
                rows, cols,
                LEFT_MARGIN, TOP_MARGIN,
                TABLE_WIDTH, SLIDE_HEIGHT - 2 * TOP_MARGIN
            )
            pptx_table = table_shape.table

            for r in range(rows):
                for c in range(cols):
                    cell = pptx_table.cell(r, c)
                    tf = cell.text_frame
                    tf.clear()
                    p = tf.paragraphs[0]
                    run = p.add_run()
                    run.text = chunk[r][c]
                    run.font.name = DEFAULT_FONT_NAME
                    run.font.size = DEFAULT_FONT_SIZE
                    p.alignment = PP_ALIGN.LEFT

                    img_data = img_chunk[r][c]
                    if img_data:
                        try:
                            left = LEFT_MARGIN + Inches(0.1) + (c * (TABLE_WIDTH / cols))
                            top = TOP_MARGIN + (r + 1) * ((SLIDE_HEIGHT - TOP_MARGIN) / (MAX_ROWS_PER_SLIDE + 1)) - Inches(0.4)
                            slide.shapes.add_picture(BytesIO(img_data), left, top, width=Inches(1.0))
                        except Exception as e:
                            print(f"Image insert failed: {e}")

    try:
        prs.save(pptx_path)
        print(f"Saved: {pptx_path}")
    except Exception as e:
        print(f"Save failed: {e}")


def extract_image_from_cell(cell):
    """
    Extracts the first image from a DOCX cell, if any.
    Returns image as bytes or None.
    """
    for para in cell.paragraphs:
        for run in para.runs:
            if run._element.xpath('.//pic:pic'):
                for blip in run._element.xpath('.//a:blip'):
                    rId = blip.get("{http://schemas.openxmlformats.org/officeDocument/2006/relationships}embed")
                    part = run.part.related_parts.get(rId)
                    if part:
                        return part.blob
    return None


if __name__ == "__main__":
    input_docx = "docx_table_boundary_test.docx"  # Replace with actual path if needed
    output_pptx = "converted_tables_clean.pptx"

    if not os.path.exists(input_docx):
        print(f"File not found: {input_docx}")
    else:
        convert_docx_tables_to_pptx(input_docx, output_pptx)
