import subprocess
import os.path as osp
import numpy as np
import scipy.misc

import cv2
import fire
from tqdm import tqdm


MOG2 = cv2.createBackgroundSubtractorMOG2
KNN = cv2.createBackgroundSubtractorKNN


class BGSubtractor:

    def __init__(self, model_factory=MOG2):
        self.model = model_factory(detectShadows=False)

    @staticmethod
    def _compute_n_frames(n_seconds=3, fps=29.97):
        n_frames = int(n_seconds * fps)
        return n_frames

    def _compute_fg(self, frame):
        mask = self.model.apply(frame)
        mask = mask[..., np.newaxis]
        return np.where(mask, frame, 0)

    def _write_result(self, fg, index, savepath):
        fpath = f'{savepath}_{index:03d}.png'
        scipy.misc.imsave(fpath, fg)

    def _display_result(self, fg):
        cv2.imshow('fg', fg)
        key = cv2.waitKey(30) & 0xff
        return key != 27

    def _bg_sub_loop(self, cap, savepath):
        n_frames = BGSubtractor._compute_n_frames()
        for i in tqdm(range(n_frames)):
            ret, frame = cap.read()
            fg = self._compute_fg(frame)
            if savepath:
                if i > 0:
                    self._write_result(fg, i, savepath)
            else:
                if not self._display_result(fg):
                    return

    def _cleanup(self, cap):
        cap.release()
        cv2.destroyAllWindows()

    def subtract_background(self, fpath, savepath=None):
        cap = cv2.VideoCapture(fpath)
        self._bg_sub_loop(cap, savepath)
        self._cleanup(cap)


def _stitch_result(savepath):
    directory = osp.dirname(savepath)
    infiles = osp.join(directory, '*.png')
    outfile = osp.join(directory, 'fg.gif')
    args = ['-delay', '0', '-loop', '0']
    cmd = ['convert', *args, infiles, outfile]
    subprocess.check_call(cmd)


def demo(fpath='./street.mp4', savepath='./results/fg'):
    bgsub = BGSubtractor()
    bgsub.subtract_background(fpath, savepath)
    _stitch_result(savepath)


if __name__ == '__main__':
    fire.Fire()
