# よくわかるGitとGitHubの使い方

[参考](https://backlog.com/ja/git-tutorial/)

## 自分の作業方法

- git add (ステージに上げる方法)

  ```
    git add <ステージにあげたいファイルのパス>
  ```

- git commit (弾丸打つ直前)
  編集したファイルをすべてステージにあげてからコミットする
  ```
    git commit -m"コミットメッセージ"
  ```
- git push (弾丸を打つ状態)
  コミット後にpushする
  ```
    git push origin <今作業しているブランチ名> or HEAD
  ```

- 最新のmasterブランチに更新する方法
  masterブランチに何も編集しているファイルがないことを確認
  ```
    git pull origin master
  ```

