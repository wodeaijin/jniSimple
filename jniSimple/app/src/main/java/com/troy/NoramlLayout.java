package com.troy;

import android.content.Context;
import android.util.AttributeSet;
import android.view.ViewGroup;
import android.widget.Scroller;

/**
 * Created by gaoqi03 on 17-4-25.
 */

public class NoramlLayout extends ViewGroup {

    Scroller mScroller;

    public NoramlLayout(Context context) {
        this(context,null);
    }

    public NoramlLayout(Context context, AttributeSet attrs) {
        this(context, attrs,0);
    }

    public NoramlLayout(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mScroller = new Scroller(context);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    protected void measureChildren(int widthMeasureSpec, int heightMeasureSpec) {
        super.measureChildren(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {

    }

    public void smoothScrollTo(int fx ,int fy){
        int x = fx - mScroller.getFinalX() ;
        int y = fy - mScroller.getFinalY();
        smoothScrollBy(x,y);
    }

    public void smoothScrollBy(int delX,int delY){
        mScroller.startScroll(mScroller.getFinalX(),mScroller.getFinalY(),delX,delY);
        invalidate();
    }

    @Override
    public void computeScroll() {
        if (mScroller.computeScrollOffset()){
            scrollTo(mScroller.getCurrX(),mScroller.getFinalY());
            postInvalidate();
        }
        super.computeScroll();
    }

    public Scroller getmScroller() {
        return mScroller;
    }

    public void setmScroller(Scroller scroller){
        mScroller = scroller;
    }


}
